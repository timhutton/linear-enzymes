// STL:
#include <vector>

struct Atom {
    bool hasBondWith( size_t iAtom );
    int x, y;
    int state;
    std::vector<size_t> bonds;
};
