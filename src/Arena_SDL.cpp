// local:
#include "Arena_SDL.hpp"

// STL:
#include <iostream>
#include <vector>
#include <sstream>

SDL_Color GetColor(char type)
{
    switch(type)
    {
        default:
        case 'e': return {255,0,0}; // red
        case 'f': return {0,255,0}; // green
        case 'a': return {220,220,0}; // yellow
        case 'b': return {128,128,128}; // gray
        case 'c': return {0,255,255}; // cyan
        case 'd': return {0,0,255}; // blue
    }
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
