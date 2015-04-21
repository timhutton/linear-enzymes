// local:
#include "Slot.hpp"

// STL:
#include <algorithm>
using namespace std;

bool Slot::isEmpty() const
{
	return this->atoms.empty();
}

void Slot::addAtom( size_t iAtom )
{
	this->atoms.push_back( iAtom );
}

void Slot::removeAtom( size_t iAtom )
{
	this->atoms.erase( remove( begin( this->atoms ), end( this->atoms ), iAtom ) );
}

size_t Slot::getAtom( size_t iiAtom )
{
    return this->atoms[ iiAtom ];
}