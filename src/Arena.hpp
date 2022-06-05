// Local:
#include "Atom.hpp"
#include "Neighborhood.hpp"
#include "Reaction.hpp"

// STL:
#include <vector>

// Arena is a square grid world containing atoms
class Arena {

    public:

        Arena( int x, int y );

        size_t addAtom( int x, int y, int type );
        bool hasBond( size_t a, size_t b ) const;
        void makeBond( size_t a, size_t b );
        void breakBond( size_t a, size_t b );
        void update();

        // accessors
        bool isOffGrid( int x, int y ) const;
        bool hasAtom( int x, int y ) const;
        int getArenaWidth() const { return this->X; }
        int getArenaHeight() const { return this->Y; }
        size_t getNumberOfAtoms() const { return this->atoms.size(); }
        Atom getAtom( size_t i ) const { return this->atoms[i]; }

    protected:

        // private variables
        const int                         X;
        const int                         Y;
        std::vector<Atom>                 atoms;
        std::vector<std::vector<std::vector<size_t>>>    grid;
        const Neighborhood                movement_neighborhood;
        const Neighborhood                chemical_neighborhood;

    private:

        // private functions
        void moveSlotIfPossible( int x, int y, int tx, int ty );
        void moveAtomsAlongBonds( int x, int y );
        void doChemistry();
        int getRandomMove() const;
        void detectEnzymes();
        bool isEnzyme( size_t iAtom, Reaction& r );
        bool collectEnzymeBits( size_t iAtom, size_t iNextAtom, std::string& s );

        // useful functions
        static bool isWithinNeighborhood( Neighborhood type, int x1, int y1, int x2, int y2 );
        static int getRandIntInclusive( int a, int b );
        static void getRandomMove( Neighborhood nhood, int& dx, int& dy );
};
