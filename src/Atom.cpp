#include "Atom.hpp"

bool Atom::hasBondWith( size_t iAtom ) {
    for( const auto& bond : bonds )
        if( bond == iAtom )
            return true;
    return false;
}