// local:
#include "Arena.hpp"
#include "utils.hpp"

// stdlib
#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace std;

//----------------------------------------------------------------------------

bool Arena::isOffGrid( int x, int y ) const {
    return x < 0 || x >= X || y < 0 || y >= Y;
}

//----------------------------------------------------------------------------

bool Arena::hasAtom( int x, int y ) const {
    if( isOffGrid(x,y ) )
        throw out_of_range("Atom not on grid");

    return !grid(x, y).empty();
}

//----------------------------------------------------------------------------

size_t Arena::addAtom( int x, int y, char type, int state ) {

    if( isOffGrid(x,y ) )
        throw out_of_range("Atom not on grid");

    Atom a;
    a.x = x;
    a.y = y;
    a.type = type;
    a.state = state;
    atoms.push_back( a );
    size_t iAtom = atoms.size()-1;

    grid(x, y).push_back( iAtom );

    return iAtom;
}

//----------------------------------------------------------------------------

void Arena::makeBond( size_t a, size_t b ) {
    if( a < 0 || a >= atoms.size() || b < 0 || b >= atoms.size() )
        throw out_of_range("Invalid atom index");
    if( a == b )
        throw invalid_argument("Cannot bond atom to itself");
    if( !isWithinNeighborhood( Neighborhood::vonNeumann, atoms[a].x, atoms[a].y, atoms[b].x, atoms[b].y ) )
        throw invalid_argument("Atoms are too far apart to be bonded");
    if( hasBond( a, b ) )
        throw invalid_argument("Atoms are already bonded");

    atoms[ a ].bonds.push_back( b );
    atoms[ b ].bonds.push_back( a );
}

//----------------------------------------------------------------------------

bool Arena::isWithinNeighborhood( Neighborhood type, int x1, int y1, int x2, int y2 ) {
    const int r2 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    switch( type ) {
        case Neighborhood::vonNeumann:  return r2 <= 1;
        case Neighborhood::Moore:       return r2 <= 2;
        case Neighborhood::vonNeumann2: return r2 <= 4;
        case Neighborhood::knight:      return r2 <= 5;
        case Neighborhood::Moore2:      return r2 <= 8;
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
    // attempt to move some slots into empty squares
    for(int i = 0; i < X * Y; i++) {
        int x = getRandIntInclusive( 0, X-1 );
        int y = getRandIntInclusive( 0, Y-1 );
        if( grid(x, y).empty() )
            continue;
        int dx, dy;
        getRandomMove( movement_neighborhood, dx, dy );
        moveSlotToEmptySquareIfPossible( x, y, x + dx, y + dy );
    }
    const bool allow_compaction = true;
    if( allow_compaction ) {
        // attempt to move some atoms to a slot they are bonded with (allows compaction)
        for(int i = 0; i < X * Y; i++) {
            int x = getRandIntInclusive( 0, X-1 );
            int y = getRandIntInclusive( 0, Y-1 );
            if( grid(x, y).empty() )
                continue;
            moveAtomsAlongBonds( x, y );
        }
        // attempt to move some atoms with exit bonds into empty slots (occasional expansion)
        for(int i = 0; i < X * Y; i++) {
            int x = getRandIntInclusive( 0, X-1 );
            int y = getRandIntInclusive( 0, Y-1 );
            if( grid(x, y).empty() )
                continue;
            int dx, dy;
            getRandomMove( Neighborhood::vonNeumann, dx, dy );
            moveAtomsOutOfSlot( x, y, x + dx, y + dy );
        }
    }

    // DEBUG: check certain conditions
    if( 1 ) {
        // does any atom with >2 bonds share a slot?
        for( const Atom& atom : atoms) {
            if( atom.bonds.size() > 2 && grid(atom.x, atom.y).size() > 1 ) {
                std::cout << "Found atom with >2 bonds found sharing a slot!" << std::endl;
            }
        }
        // does any bond cross another?
        for(int x = 0; x < X - 1; x++) {
            for(int y = 0; y < Y - 1; y++) {
                if( hasBond( x, y, x+1, y+1) && hasBond(x+1,y,x,y+1) ) {
                    std::cout << "Found crossed bonds!" << std::endl;
                }
            }
        }
    }

    // find chemical reactions
    //doChemistry();
}

//----------------------------------------------------------------------------

void Arena::moveSlotToEmptySquareIfPossible( int x, int y, int tx, int ty ) {
    if( isOffGrid(tx, ty) || !grid(tx, ty).empty() )
        return;
    // would this move stretch any bond too far?
    for( const size_t iAtom : grid(x, y) ) {
        for( const size_t iOtherAtom : atoms[ iAtom ].bonds ) {
            const Atom& atom  = atoms[ iAtom ];
            const Atom& otherAtom = atoms[ iOtherAtom ];
            if( !isWithinNeighborhood( bond_neighborhood, tx, ty, otherAtom.x, otherAtom.y ) ) {
                return;
            }
            // (bonds to atoms already in the same slot don't actually need to be checked)
            // (atoms in the slot being moved don't actually need to be checked)
        }
    }
    for( const size_t iAtom : grid(x, y) ) {
        Atom &atom = atoms[ iAtom ];
        atom.x = tx;
        atom.y = ty;
        grid(atom.x, atom.y).push_back( iAtom );
    }
    grid(x, y).clear();
}

//----------------------------------------------------------------------------

size_t Arena::getNumExitBonds( int x, int y) {
    size_t num_exit_bonds = 0;
    for( const size_t iAtom : grid(x, y) ) {
        for( const size_t iOtherAtom : atoms[ iAtom ].bonds ) {
            const Atom& otherAtom = atoms[iOtherAtom];
            if( otherAtom.x != x || otherAtom.y != y )
                num_exit_bonds++;
        }
    }
    return num_exit_bonds;
}

//----------------------------------------------------------------------------

void Arena::moveAtomsAlongBonds( int x, int y ) {
    if( grid(x, y).empty() )
        return;
    // pick an atom in this slot at random
    const size_t iiAtom = getRandIntInclusive(0, grid(x, y).size()-1);
    const size_t iAtom = grid(x, y)[iiAtom];
    Atom& atom = atoms[iAtom];
    if( atom.bonds.empty() )
        return;
    // don't move if atom has more than two
    if( atom.bonds.size() > 2 )
        return;
    // pick a bonded atom at random to move to
    const size_t iBond = getRandIntInclusive(0, atom.bonds.size()-1 );
    const size_t iBondedAtom = atom.bonds[iBond];
    const Atom& bonded_atom = atoms[iBondedAtom];
    const int tx = bonded_atom.x;
    const int ty = bonded_atom.y;
    // must be in a different location
    if( tx == x && ty == y )
        return;
    // would this overfill the target slot?
    if( grid(tx, ty).size() >= max_slot_capacity )
        return;
    // would this move stretch any bond too far?
    for( const size_t& iBondedAtom : atom.bonds ) {
        const Atom& bonded_atom = atoms[ iBondedAtom ];
        if( !isWithinNeighborhood( bond_neighborhood, tx, ty, bonded_atom.x, bonded_atom.y ) ) {
            return;
        }
    }
    // don't move to a slot with more than two exit bonds
    if( getNumExitBonds( tx, ty ) > 2 )
        return;
    // remove the atom from its current slot
    grid(x, y).erase( grid(x, y).begin() + iiAtom );
    // put it in the new slot
    grid(tx, ty).push_back(iAtom);
    atom.x = tx;
    atom.y = ty;
}

//----------------------------------------------------------------------------

void Arena::moveAtomsOutOfSlot( int x, int y, int tx, int ty ) {
    // move an atom with an exit bond into an empty slot
    if( grid(x, y).empty() || isOffGrid(tx, ty) || !grid(tx, ty).empty() )
        return;
    // pick an atom in this slot at random
    const size_t iiAtom = getRandIntInclusive(0, grid(x, y).size()-1);
    const size_t iAtom = grid(x, y)[iiAtom];
    Atom& atom = atoms[iAtom];
    if( atom.bonds.empty() )
        return;
    // would this move stretch any bond too far?
    // does this atom have any exit bonds?
    // would it leave the atom with any diagonal bonds? (can cause crossing and a cascade of topological changes)
    bool exit_bond_found = false;
    for( const size_t& iBondedAtom : atom.bonds ) {
        const Atom& bonded_atom = atoms[ iBondedAtom ];
        if( !isWithinNeighborhood( Neighborhood::vonNeumann, tx, ty, bonded_atom.x, bonded_atom.y ) ) {
            return;
        }
        if( bonded_atom.x != x || bonded_atom.y != y )
            exit_bond_found = true;
    }
    if( !exit_bond_found )
        return;
    // remove the atom from its current slot
    grid(x, y).erase( grid(x, y).begin() + iiAtom );
    // put it in the new slot
    grid(tx, ty).push_back(iAtom);
    atom.x = tx;
    atom.y = ty;
}

//---------------------------------------------------------------------------

bool Arena::hasBond( size_t a, size_t b ) const {
    for( const size_t& iAtom : atoms[ a ].bonds ) {
        if( iAtom == b )
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------

bool Arena::hasBond( int x1, int y1, int x2, int y2 ) const {
    for( const size_t& iAtom : grid(x1, y1) ) {
        const Atom& atom = atoms[iAtom];
        for( const size_t iOtherAtom : atom.bonds ) {
            const Atom& otherAtom = atoms[iOtherAtom];
            if( otherAtom.x == x2 && otherAtom.y == y2 )
                return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------

void Arena::doChemistry() {
/*
    const int num_vN = 5;
    const int vNx[ num_vN ] = { 0,  0,  1,  0, -1 }; // CNESW
    const int vNy[ num_vN ] = { 0, -1,  0,  1,  0 };

    Reaction r;
    int rr = 5; // reaction radius
    for( size_t iAtom = 0; iAtom < atoms.size(); ++iAtom ) {
        if( isEnzyme( iAtom, r ) ) {
            // catalyse this reaction locally
            int ax = atoms[ iAtom ].x;
            int ay = atoms[ iAtom ].y;
            for( int x = ax - rr; x <= ax + rr; ++x ) {
                for( int y = ay - rr; y <= ay + rr; ++y ) {
                    if( isOffGrid(x,y) || grid(x, y).empty() ) continue;
                    for( int dir = 0; dir < num_vN; ++dir ) {
                        int ox = x + vNx[ dir ];
                        int oy = y + vNy[ dir ];
                        if( isOffGrid(ox,oy) || grid[ox][oy].empty() ) continue;
                        for( int iiA = 0; iiA < grid(x, y).size(); ++iiA ) {
                            size_t iA = grid(x, y)[iiA];
                            Atom& a = atoms[ iA ];
                            for( int iiB = 0; iiB < grid[ox][oy].size(); ++iiB ) {
                                size_t iB = grid[ox][oy][iiB];
                                if( iB == iA ) continue;
                                Atom& b = atoms[ iB ];
                                // check the reaction matches the reactants
                                if( a.state != r.a_pre || b.state != r.b_pre ) continue;
                                bool bonded = a.hasBondWith( iB );
                                if( bonded != r.bonded_pre ) continue;
                                // apply the reaction
                                a.state = r.a_post;
                                b.state = r.b_post;
                                if( !bonded && r.bonded_post )
                                    makeBond( iA, iB );
                                else if( bonded && !r.bonded_post )
                                    breakBond( iA, iB );
                            }
                        }
                    }
                }
            }
        }
    }*/
}

//----------------------------------------------------------------------------

bool Arena::isEnzyme( size_t iAtom, Reaction& r ) {
    // test this atom to see if valid as the site of an enzyme:
    /*const Atom& atom = atoms[ iAtom ];
    if( atom.bonds.size() != 1 ) return false;
    string s;
    s += atom.type;
    if( collectEnzymeBits( iAtom, atom.bonds.front(), s ) ) {
        r = Reaction( s );
        return true;
    }*/
    return false;
}

//----------------------------------------------------------------------------

bool Arena::collectEnzymeBits( size_t iAtom, size_t iNextAtom, string& s ) {
    // follow this bond, collecting the values in s if still a valid enzyme
    /*const Atom& next_atom = atoms[ iNextAtom ];
    if( next_atom.bonds.size() > 2 ) return false;
    s += next_atom.type;
    if( !Reaction::validSoFar( s ) )
        return false;
    if( Reaction::isValid( s ) )
        return true;

    for( const size_t& iNextNextAtom : next_atom.bonds ) {
        if( iNextNextAtom != iAtom )
            return collectEnzymeBits( iNextAtom, iNextNextAtom, s );
    }*/
    return false; // string is too short to interpret as a reaction
}

//----------------------------------------------------------------------------

void Arena::breakBond( size_t iA, size_t iB ) {
    Atom& a = atoms[ iA ];
    Atom& b = atoms[ iB ];
    for( vector<size_t>::const_iterator it = begin( a.bonds ); it != end( a.bonds ); ++it ) {
        if( *it == iB ) {
            a.bonds.erase( it );
            break;
        }
    }
    for( vector<size_t>::const_iterator it = begin( b.bonds ); it != end( b.bonds ); ++it ) {
        if( *it == iA ) {
            b.bonds.erase( it );
            break;
        }
    }
}

//----------------------------------------------------------------------------

