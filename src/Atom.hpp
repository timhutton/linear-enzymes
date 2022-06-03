// local:
#include "Bond.hpp"

// STL:
#include <vector>

// using int2 = std::array<int, 2>; // TODO

struct Atom {
    bool hasBondWith( size_t iAtom );
    int x, y;
    int state;
    std::vector<Bond> bonds;
};
