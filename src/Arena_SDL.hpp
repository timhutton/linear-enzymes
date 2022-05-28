// local:
#include "Arena.hpp"

// SDL2:
#include <SDL2/SDL.h>

class Arena_SDL : public Arena
{
public:

    void Draw(SDL_Renderer* renderer,float scale);

};
