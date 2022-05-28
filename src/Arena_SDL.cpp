// local:
#include "Arena_SDL.hpp"

// STL:
#include <vector>
#include <sstream>
using namespace std;

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
    static const int OFFSET = 20;

    // clear the background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // draw the atoms
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for(const Atom& atom : atoms)
    {
        // draw the cell
        SDL_Color color = GetColor("abcdef"[atom.state%6]);
        const int alpha = (atom.state == 4) ? 10 : 255;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
        SDL_Rect r;
        r.x = atom.x*scale + OFFSET;
        r.y = atom.y*scale + OFFSET;
        r.w = scale;
        r.h = scale;
        SDL_RenderFillRect(renderer, &r);
    }

    // draw the bonds
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
    for(int iAtom = 0; iAtom < atoms.size(); iAtom++)
    {
        // draw the atoms's bonds as lines
        for(const Bond& bond : atoms[iAtom].bonds)
        {
            if(bond.iAtom < iAtom) continue; // (avoid drawing the same line more than once)

            SDL_RenderDrawLine(renderer,
                int(atoms[iAtom].x*scale)+OFFSET,
                int(atoms[iAtom].y*scale)+OFFSET,
                int(atoms[bond.iAtom].x*scale)+OFFSET,
                int(atoms[bond.iAtom].y*scale)+OFFSET);
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
