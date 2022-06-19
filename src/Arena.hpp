// Local:
#include "Atom.hpp"
#include "Neighborhood.hpp"
#include "Reaction.hpp"

// STL:
#include <array>
#include <vector>

template<typename T, size_t X, size_t Y>
class Array2D {
    public:
        T& operator()(size_t x, size_t y)             { return arr[ x * Y + y ]; }
        const T& operator()(size_t x, size_t y) const { return arr[ x * Y + y ]; }
    private:
        std::array<T, X*Y> arr;
};

// Arena is a square grid world containing atoms
class Arena {

    public:

        static constexpr int X = 102;
        static constexpr int Y = 80;

        size_t addAtom( int x, int y, char type, int state );
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
        size_t getNumberOfAtoms(int x, int y) const { return grid(x, y).size(); }
        Atom getAtom( size_t i ) const { return this->atoms[i]; }
        size_t getAtomIndex( int x, int y, size_t i ) const { return grid(x, y)[i]; }

    protected:

        Array2D<std::vector<size_t>, X, Y> grid;
        std::vector<Atom> atoms;

        static constexpr Neighborhood movement_neighborhood = Neighborhood::Moore;
        static constexpr Neighborhood bond_neighborhood = Neighborhood::Moore;
        static constexpr size_t max_slot_capacity = Reaction::num_digits-1; // (don't want enzymes in a single slot)

    private:

        // private functions
        void moveSlotToEmptySquareIfPossible( int x, int y, int tx, int ty );
        void moveAtomsAlongBonds( int x, int y );
        void moveAtomsOutOfSlot( int x, int y, int tx, int ty );
        void doChemistry();
        int getRandomMove() const;
        bool isEnzyme( size_t iAtom, Reaction& r );
        bool collectEnzymeBits( size_t iAtom, size_t iNextAtom, std::string& s );
        size_t getNumExitBonds( int x, int y);
        bool hasBond( int x1, int y1, int x2, int y2 ) const;

        // useful functions
        static bool isWithinNeighborhood( Neighborhood type, int x1, int y1, int x2, int y2 );
        static void getRandomMove( Neighborhood nhood, int& dx, int& dy );
};
