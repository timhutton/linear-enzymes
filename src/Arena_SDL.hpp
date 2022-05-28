// local:
#include "Arena.hpp"

// SDL2:
#include <SDL2/SDL.h>

class Arena_SDL : public Arena
{
public:
    Arena_SDL( int x, int y ) : Arena(x,y) {}

    void Draw(SDL_Renderer* renderer,float scale);
};
