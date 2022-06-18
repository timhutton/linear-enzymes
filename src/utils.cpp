// Local:
#include "utils.hpp"

// stdlib:
#include <cstdlib>

int getRandIntInclusive( int a, int b )
{
    return a + rand() % ( b - a + 1 );
}
