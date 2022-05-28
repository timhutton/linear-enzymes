#ifndef BOND_HPP
#define BOND_HPP

// local:
#include "Neighborhood.hpp"

// Stdlib:
#include <cstdlib>

struct Bond {
    size_t iAtom;
    Neighborhood range;
};

#endif