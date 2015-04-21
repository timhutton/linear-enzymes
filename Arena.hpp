// Local:
#include "Atom.hpp"
#include "Slot.hpp"
#include "Neighborhood.hpp"
#include "Bond.hpp"
#include "Reaction.hpp"

// STL:
#include <vector>

// Arena is a square grid world containing atoms
class Arena {

	public:
        
        Arena( int x, int y );

		size_t addAtom( int x, int y, int type );
		void makeBond( size_t a, size_t b, Neighborhood range );
        void breakBond( size_t a, size_t b );
        void update();

        // accessors
        bool isOffGrid( int x, int y ) const;
        bool hasAtom( int x, int y ) const;
        bool canPlaceAtom( int x, int y ) const;
        int getArenaWidth() const { return this->X; }
        int getArenaHeight() const { return this->Y; }
        size_t getNumberOfAtoms() const { return this->atoms.size(); }
        Atom getAtom( size_t i ) const { return this->atoms[i]; }
        size_t getNumberOfGroups() const { return this->groups.size(); }
	
	private:

        // typedefs
        struct Group { std::vector<size_t> atoms; };
        enum MovementMethod { JustAtoms      // atoms can move individually
                            , AllGroups      // all subgraphs of atoms can move individually
                            , MPEGSpace      // space itself moves around in large blocks
                            , MPEGMolecules  // molecules are divided spatially into movement blocks on the fly
                            };
        enum Proximity { PassThrough         // atoms can pass through each other; each slot can have multiple atoms
                       , SingleOccupancy     // atoms cannot occupy the same slot
                       };

        // private variables
        const int                         X;
        const int                         Y;
		std::vector<Atom>                 atoms;
        std::vector<std::vector<Slot>>    grid;
		std::vector<Group>                groups;
        const MovementMethod              movement_method;
        const Neighborhood                movement_neighborhood;
        const Neighborhood                chemical_neighborhood;
        const Proximity                   proximity;

        // private functions
        void addAllGroupsForNewBond( size_t a, size_t b );
        void removeGroupsWithOneButNotTheOther( size_t a, size_t b );
        void combineGroupsInvolvingTheseIntoOne( size_t a, size_t b );
        bool moveGroupIfPossible( const Group& group, int dx, int dy );
        bool moveBlockIfPossible( int x, int y, int w, int h, int dx, int dy );
        void moveBlocksInGroup( const Group& group );
        void moveBlocksInGroup( const Group& group, int x, int y, int w, int h );
        bool moveMembersOfGroupInBlockIfPossible( const Group& group, int x, int y, int w, int h, int dx, int dy  );
        void doChemistry();
        bool hasBond( size_t a, size_t b ) const;
        int getRandomMove() const;
        void detectEnzymes();
        bool isEnzyme( size_t iAtom, Reaction& r );
        bool collectEnzymeBits( size_t iAtom, const Bond& bond, std::string& s );

        // useful functions
        static bool isWithinNeighborhood( Neighborhood type, int x1, int y1, int x2, int y2 );
        static int getRandIntInclusive( int a, int b );
        static void getRandomMove( Neighborhood nhood, int& dx, int& dy );
};
