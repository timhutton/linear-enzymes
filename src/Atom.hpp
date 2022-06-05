// STL:
#include <vector>

struct Atom {
    bool hasBondWith( size_t iAtom );
    int x, y;
    char type;
    int state;
    std::vector<size_t> bonds;
};
