// local:
#include "Arena.hpp"

// stdlib
#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace std;

//----------------------------------------------------------------------------

Arena::Arena(int x, int y)
    : X( x )
    , Y( y )
    , movement_method( MovementMethod::JustAtoms )
    //, movement_method( MovementMethod::MPEGSpace )
    , movement_neighborhood( Neighborhood::vonNeumann ) // currently only vonNeumann supported
    , chemical_neighborhood( Neighborhood::vonNeumann )
    //, proximity( Proximity::SingleOccupancy )
    //, proximity( Proximity::PassThrough )
    , proximity( Proximity::Compact )
{
    this->grid = vector<vector<vector<size_t>>>( X, vector<vector<size_t>>( Y ) );
}

//----------------------------------------------------------------------------

bool Arena::isOffGrid( int x, int y ) const {
    return x < 0 || x >= X || y < 0 || y >= Y;
}

//----------------------------------------------------------------------------

bool Arena::hasAtom( int x, int y ) const {
    if( isOffGrid(x,y ) )
        throw out_of_range("Atom not on grid");

    return !this->grid[x][y].empty();
}

//----------------------------------------------------------------------------

bool Arena::canPlaceAtom( int x, int y ) const {
    switch( this->proximity ) {
        default:
        case PassThrough:
        case Compact:
            return true;
        case SingleOccupancy:
            return this->grid[ x ][ y ].empty();
    }
}

//----------------------------------------------------------------------------

size_t Arena::addAtom( int x, int y, int state ) {

    if( isOffGrid(x,y ) )
        throw out_of_range("Atom not on grid");

    Atom a;
    a.x = x;
    a.y = y;
    a.state = state;
    this->atoms.push_back( a );
    size_t iAtom = this->atoms.size()-1;

    this->grid[x][y].push_back( iAtom );

    return iAtom;
}

//----------------------------------------------------------------------------

void Arena::makeBond( size_t a, size_t b, Neighborhood range ) {
    if( a < 0 || a >= atoms.size() || b < 0 || b >= atoms.size() )
        throw out_of_range("Invalid atom index");
    if( a == b )
        throw invalid_argument("Cannot bond atom to itself");
    if( !isWithinNeighborhood( range, this->atoms[a].x, this->atoms[a].y, this->atoms[b].x, this->atoms[b].y ) )
        throw invalid_argument("Atoms are too far apart to be bonded");
    if( hasBond( a, b ) )
        throw invalid_argument("Atoms are already bonded");

    Bond ab = { b, range };
    this->atoms[ a ].bonds.push_back( ab );
    Bond ba = { a, range };
    this->atoms[ b ].bonds.push_back( ba );
}

//----------------------------------------------------------------------------

bool Arena::isWithinNeighborhood( Neighborhood type, int x1, int y1, int x2, int y2 ) {
    const int r2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    switch( type ) {
        case vonNeumann:  return r2 <= 1;
        case Moore:       return r2 <= 2;
        case vonNeumann2: return r2 <= 4;
        case knight:      return r2 <= 5;
        case Moore2:      return r2 <= 8;
    }
    throw out_of_range("unexpected enum");
}

//----------------------------------------------------------------------------

void Arena::getRandomMove( Neighborhood nhood, int &dx, int &dy ) {
    switch( nhood ) {
        case Neighborhood::vonNeumann: {
            const int vNx[4] = {  0,  1,  0, -1 }; // clockwise from North
            const int vNy[4] = { -1,  0,  1,  0 };
            const int iMove = getRandIntInclusive(0,3);
            dx = vNx[ iMove ];
            dy = vNy[ iMove ];
            break;
        }
        case Neighborhood::Moore: {
            const int Mx[8] = {  0,  1,  1,  1,  0, -1, -1, -1 }; // clockwise from North
            const int My[8] = { -1, -1,  0,  1,  1,  1,  0, -1 };
            const int iMove = getRandIntInclusive(0,7);
            dx = Mx[ iMove ];
            dy = My[ iMove ];
            break;
        }
        default: throw out_of_range("Unsupported neighborhood");
    }
}

//----------------------------------------------------------------------------

void Arena::update() {
    switch( this->movement_method ) {
        case JustAtoms:
            // attempt to move every slot
            for(int x = 0; x < X; x++ ) {
                for(int y = 0; y < Y; y++ ) {
                    if( this->grid[x][y].empty() )
                        continue;
                    int dx, dy;
                    getRandomMove( this->movement_neighborhood, dx, dy );
                    moveAtomsIfPossible( x, y, dx, dy );
                }
            }
            break;
        case MPEGSpace: {
            const int max_block_size = 100;
            for( int i = 0; i < 10; ++i ) {
                // attempt to move a block
                int x = getRandIntInclusive( 0, this->X-1 );
                int y = getRandIntInclusive( 0, this->Y-1 );
                int w = getRandIntInclusive( 1, std::min(max_block_size, this->X-x) );
                int h = getRandIntInclusive( 1, std::min(max_block_size, this->Y-y) );
                int dx, dy;
                getRandomMove( this->movement_neighborhood, dx, dy );
                moveBlockIfPossible( x, y, w, h, dx, dy );
            }
            break;
        }
    }

    // find chemical reactions
    //doChemistry();
}

//----------------------------------------------------------------------------

void Arena::doChemistry() {
    detectEnzymes();
    for( int x = 0; x < this->X; ++x ) {
        for( int y = 0; y < this->Y; ++y ) {
            if( this->grid[x][y].empty() ) continue;
            int dx, dy;
            getRandomMove( this->chemical_neighborhood, dx, dy );
            int tx = x + dx;
            int ty = y + dy;
            if( isOffGrid( tx, ty ) || this->grid[tx][ty].empty() ) continue;
            /* TODO:
            size_t iAtomA = this->grid[x][y].iAtom;
            size_t iAtomB = this->grid[tx][ty].iAtom;
            Atom& a = this->atoms[ iAtomA ];
            Atom& b = this->atoms[ iAtomB ];
            if( !hasBond( iAtomA, iAtomB ) && a.type == b.type && a.bonds.size() + b.bonds.size() < 2 ) {
                Neighborhood bond_range = Neighborhood::Moore;
                makeBond( iAtomA, iAtomB, bond_range );
            }*/
        }
    }
}

//----------------------------------------------------------------------------

bool Arena::moveAtomsIfPossible( int x, int y, int dx, int dy ) {
    if( isOffGrid(x+dx, y+dy) )
        return false;
    // would this move stretch any bond too far?
    for( const size_t& iAtomIn : this->grid[x][y] ) {
        for( const Bond& bond : this->atoms[ iAtomIn ].bonds ) {
            const size_t& iAtomOut = bond.iAtom;
            const Atom& atomIn  = this->atoms[ iAtomIn ];
            const Atom& atomOut = this->atoms[ iAtomOut ];
            if( !isWithinNeighborhood( bond.range, atomIn.x + dx, atomIn.y + dy, atomOut.x, atomOut.y ) ) {
                return false;
            }
        }
    }
    for( const auto& iAtom : this->grid[x][y] ) {
        Atom &atom = this->atoms[ iAtom ];
        atom.x += dx;
        atom.y += dy;
        this->grid[ atom.x ][ atom.y ].push_back( iAtom );
    }
    this->grid[x][y].clear();
    return true;
}

//----------------------------------------------------------------------------

bool Arena::moveBlockIfPossible( int x, int y, int w, int h, int dx, int dy ) {
    const int left = x;
    const int right = x + w - 1;
    const int top = y;
    const int bottom = y + h - 1;
    if( isOffGrid( left, top ) || isOffGrid( right, bottom ) )
        throw out_of_range("Attempt to move block that is not wholly on the grid");
    // overlap test along front edge:
    int x1, y1, x2, y2;
    if( dx == 1 )       { x1 = x2 = right;  y1 = top;  y2 = bottom; }
    else if( dx == -1 ) { x1 = x2 = left;   y1 = top;  y2 = bottom; }
    else if( dy == 1 )  { y1 = y2 = bottom; x1 = left; x2 = right;  }
    else if( dy == -1 ) { y1 = y2 = top;    x1 = left; x2 = right;  }
    for( int sx = x1; sx <= x2; ++sx ) {
        for( int sy = y1; sy <= y2; ++sy ) {
            if( this->grid[sx][sy].empty() )
                continue;
            int tx = sx + dx;
            int ty = sy + dy;
            if( isOffGrid(tx,ty) || !this->grid[tx][ty].empty() )
                return false;
        }
    }
    // bond test along back and side edges:
    // TODO: improve test to reject leading edge
    // TODO: find better way of traversing non-leading edges
    const bool forbid_bonds_from_stretching = true;
    if(forbid_bonds_from_stretching) {
        for( int sx = left; sx <= right; ++sx ) {
            for( int sy = top; sy <= bottom; ++sy ) {
                if( sx > left && sx < right && sy > top && sy < bottom )
                    continue; // not on the edge of the block
                if( this->grid[sx][sy].empty() )
                    continue;
                for(int iMover = 0; iMover < this->grid[sx][sy].size(); iMover++) {
                    const Atom& a = this->atoms[ this->grid[sx][sy][iMover] ];
                    for( const Bond& bond : a.bonds ) {
                        const size_t iAtomB = bond.iAtom;
                        const Atom& b = this->atoms[ iAtomB ];
                        if( b.x >= left && b.x <= right && b.y >= top && b.y <= bottom )
                            continue; // atom B is also within the block
                        if( !isWithinNeighborhood( bond.range, sx + dx, sy + dy, b.x, b.y ) )
                            return false; // would over-stretch this bond
                    }
                }
            }
        }
    }
    // move the block
    vector<size_t> movers;
    for( int sx = left; sx <= right; ++sx ) {
        for( int sy = top; sy <= bottom; ++sy ) {
            if( this->grid[sx][sy].empty() )
                continue;
            for(int iMover = 0; iMover < this->grid[sx][sy].size(); iMover++) {
                movers.push_back( this->grid[sx][sy][iMover] );
            }
            this->grid[sx][sy].clear();
        }
    }
    for( const size_t& iAtom : movers ) {
        Atom& a = this->atoms[ iAtom ];
        a.x += dx;
        a.y += dy;
        if( isOffGrid( a.x, a.y ) )
            throw std::runtime_error("attempt to move atom off-grid");
        this->grid[ a.x ][ a.y ].push_back( iAtom );
    }
    return true;
}

//----------------------------------------------------------------------------

int Arena::getRandIntInclusive( int a, int b )
{
    return a + rand() % ( b - a + 1 );
}

//----------------------------------------------------------------------------

bool Arena::hasBond( size_t a, size_t b ) const {
    for( const Bond& bond : this->atoms[ a ].bonds ) {
        if( bond.iAtom == b )
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------

void Arena::detectEnzymes() {

    const int num_vN = 5;
    const int vNx[ num_vN ] = { 0,  0,  1,  0, -1 }; // CNESW
    const int vNy[ num_vN ] = { 0, -1,  0,  1,  0 };

    Reaction r;
    int rr = 50;
    for( size_t iAtom = 0; iAtom < this->atoms.size(); ++iAtom ) {
        if( isEnzyme( iAtom, r ) ) {
            // catalyse this reaction locally for a while
            int ax = this->atoms[ iAtom ].x;
            int ay = this->atoms[ iAtom ].y;
            for( int x = ax - rr; x <= ax + rr; ++x ) {
                for( int y = ay - rr; y <= ay + rr; ++y ) {
                    for( int dir = 0; dir < num_vN; ++dir ) {
                        int ox = x + vNx[ dir ];
                        int oy = y + vNy[ dir ];
                        if( isOffGrid(x,y) || isOffGrid(ox,oy) ) continue;
                        if( this->grid[x][y].empty() || this->grid[ox][oy].empty() ) continue;
                        for( int iiA = 0; iiA < this->grid[x][y].size(); ++iiA ) {
                            size_t iA = this->grid[x][y][iiA];
                            Atom& a = this->atoms[ iA ];
                            for( int iiB = 0; iiB < this->grid[ox][oy].size(); ++iiB ) {
                                size_t iB = this->grid[ox][oy][iiB];
                                if( iB == iA ) continue;
                                Atom& b = this->atoms[ iB ];
                                // check the reaction matches the reactants
                                if( a.state != r.a_pre || b.state != r.b_pre ) continue;
                                bool bonded = a.hasBondWith( iB );
                                if( bonded != r.bonded_pre ) continue;
                                // apply the reaction
                                a.state = r.a_post;
                                b.state = r.b_post;
                                if( !bonded && r.bonded_post )
                                    makeBond( iA, iB, Neighborhood::Moore );
                                else if( bonded && !r.bonded_post )
                                    breakBond( iA, iB );
                            }
                        }
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------

bool Arena::isEnzyme( size_t iAtom, Reaction& r ) {
    // test this atom to see if valid as the site of an enzyme:
    const Atom& atom = this->atoms[ iAtom ];
    if( atom.state != 1 ) return false;
    for( const auto& bond: atom.bonds ) {
        string s;
        s += atom.state + '0';
        if( collectEnzymeBits( iAtom, bond, s ) ) {
            r = Reaction( s );
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------

bool Arena::collectEnzymeBits( size_t iAtom, const Bond& bond, string& s ) {
    // follow this bond, collecting the values in s if still a valid enzyme
    const Atom& new_atom = this->atoms[ bond.iAtom ];
    if( new_atom.bonds.size() > 2 ) return false;
    s += new_atom.state + '0';
    if( !Reaction::validSoFar( s ) )
        return false;
    if( Reaction::isValid( s ) )
        return true;

    for( const auto& new_bond : new_atom.bonds ) {
        if( new_bond.iAtom != iAtom )
            return collectEnzymeBits( bond.iAtom, new_bond, s );
    }
    return false;
}

//----------------------------------------------------------------------------

void Arena::breakBond( size_t iA, size_t iB ) {
    Atom& a = this->atoms[ iA ];
    Atom& b = this->atoms[ iB ];
    for( vector<Bond>::const_iterator it = begin( a.bonds ); it != end( a.bonds ); ++it ) {
        if( it->iAtom == iB ) {
            a.bonds.erase( it );
            break;
        }
    }
    for( vector<Bond>::const_iterator it = begin( b.bonds ); it != end( b.bonds ); ++it ) {
        if( it->iAtom == iA ) {
            b.bonds.erase( it );
            break;
        }
    }
}

//----------------------------------------------------------------------------

