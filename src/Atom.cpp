#include "Atom.hpp"

bool Atom::hasBondWith( size_t iAtom ) {
    for( const auto& bond : this->bonds )
        if( bond.iAtom == iAtom )
            return true;
    return false;
}