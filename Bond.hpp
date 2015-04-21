#ifndef BOND_HPP
#define BOND_HPP

// local:
#include "Neighborhood.hpp"

struct Bond { 
	size_t iAtom; 
	Neighborhood range; 
};

#endif