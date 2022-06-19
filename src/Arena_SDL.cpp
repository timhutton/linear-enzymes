// local:
#include "Arena_SDL.hpp"

// STL:
#include <iostream>
#include <vector>
#include <sstream>

SDL_Color GetColor(char type)
{
    constexpr int N = 15;
    constexpr SDL_Color colors[N] = {{0,240,246}, {251,64,40}, {1,58,168}, {202,252,63}, {122,42,194},
                                     {52,107,0}, {255,90,218}, {0,170,141}, {231,0,122}, {209,255,193},
                                     {166,0,39}, {1,114,182}, {197,100,0}, {187,125,159}, {118,58,87}};
    return colors[ (type - 'a') % N ];
}

void Arena_SDL::Draw(SDL_Renderer* renderer,float scale)
{
    // clear the background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // draw the atoms
    for(int x = 0; x < X; x++) {
        for(int y = 0; y < Y; y++) {
            if( grid(x, y).empty() ) continue;
            const Atom& atom = atoms[ grid(x, y).front() ];
            // draw the atom
            SDL_Color color = GetColor(atom.type);
            const int alpha = (atom.state == 0) ? 10 : 255;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
            SDL_Rect r;
            r.x = atom.x*scale;
            r.y = atom.y*scale;
            r.w = scale;
            r.h = scale;
            SDL_RenderFillRect(renderer, &r);
        }
    }

    // draw the bonds
    const bool draw_bonds = true;
    if(draw_bonds) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
        for(int iAtom = 0; iAtom < atoms.size(); iAtom++)
        {
            // draw the atoms's bonds as lines
            for(const size_t iOtherAtom : atoms[iAtom].bonds)
            {
                if(iOtherAtom < iAtom) continue; // (avoid drawing the same line more than once)

                SDL_RenderDrawLine(renderer,
                    atoms[iAtom].x * scale + scale / 2,
                    atoms[iAtom].y * scale + scale / 2,
                    atoms[iOtherAtom].x * scale + scale / 2,
                    atoms[iOtherAtom].y * scale + scale / 2);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
