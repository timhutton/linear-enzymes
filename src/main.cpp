// SDL2:
#include <SDL2/SDL.h>

// Emscripten:
#include <emscripten.h>

// stdlib:
#include <cstdlib>
#include <string>

// local:
#include "Arena_SDL.hpp"

struct context
{
    SDL_Renderer *renderer;
    Arena_SDL *arena;
    int iteration;
    float scale;
};

void seed(Arena& arena);

void mainloop(void *arg)
{
    context *ctx = static_cast<context*>(arg);
    SDL_Renderer *renderer = ctx->renderer;

    const size_t iterations_per_render = 5;
    for(size_t i = 0; i < iterations_per_render; ++i)
    {
        ctx->arena->update();
    }
    ctx->arena->Draw(renderer, ctx->scale);

    SDL_RenderPresent(renderer);

    ctx->iteration++;
}

int main()
{
    const int SIDE_X = 70;
    const int SIDE_Y = 70;
    const float scale = 10.0f;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(SIDE_X*scale, SIDE_Y*scale, 0, &window, &renderer);

    Arena_SDL arena(SIDE_X,SIDE_Y);
    seed(arena);

    context ctx;
    ctx.renderer = renderer;
    ctx.iteration = 0;
    ctx.arena = &arena;
    ctx.scale = scale;

    const int simulate_infinite_loop = 1; // call the function repeatedly
    const int fps = -1; // call the function as fast as the browser wants to render (typically 60fps)
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

//----------------------------------------------------------------------------

void seed(Arena& arena)
{
    // add a cell
    if( 1 ) {
        // some enzymes
        std::vector<std::string> vs;
        vs.push_back( Reaction( 3, false, 4, 8, true, 5 ).getString() );
        vs.push_back( Reaction( 5, false, 4, 11, true, 10 ).getString() );
        vs.push_back( Reaction( 11, true, 6, 6, true, 11 ).getString() );
        vs.push_back( Reaction( 11, true, 7, 7, true, 11 ).getString() );
        vs.push_back( Reaction( 11, true, 8, 8, true, 11 ).getString() );
        vs.push_back( Reaction( 0, true, 11, 6, true, 12 ).getString() );
        vs.push_back( Reaction( 1, true, 11, 7, true, 13 ).getString() );
        vs.push_back( Reaction( 12, true, 6, 6, true, 12 ).getString() );
        vs.push_back( Reaction( 12, true, 7, 7, true, 12 ).getString() );
        vs.push_back( Reaction( 12, true, 8, 8, true, 12 ).getString() );
        vs.push_back( Reaction( 13, true, 6, 6, true, 13 ).getString() );
        vs.push_back( Reaction( 13, true, 7, 7, true, 13 ).getString() );
        vs.push_back( Reaction( 13, true, 8, 8, true, 13 ).getString() );
        const std::string dna = "4101011230123012312301220103201035";
        vs.push_back( dna );
        vs.push_back( Reaction( 12, true, 10, 6, true, 5 ).getString() );
        vs.push_back( Reaction( 13, true, 10, 7, true, 5 ).getString() );
        vs.push_back( Reaction( 2, true, 11, 2, true, 14 ).getString() );
        vs.push_back( Reaction( 14, true, 6, 0, true, 14 ).getString() );
        vs.push_back( Reaction( 14, true, 7, 1, true, 14 ).getString() );
        vs.push_back( Reaction( 14, true, 8, 3, false, 15 ).getString() );
        vs.push_back( Reaction( 15, false, 4, 14, true, 3 ).getString() );
        vs.push_back( Reaction( 14, true, 10, 2, false, 9 ).getString() );
        int x = 11;
        size_t e, f;
        for( const std::string& s : vs ) {
            const int N = s.length();
            int y = 10;
            size_t a = arena.addAtom( x, y, s[0] - '0' + 'a', 1 );
            if( s == dna )
                e = a;
            for( int i = 1; i < N; ++i ) {
                y++;
                size_t a2 = arena.addAtom( x, y, s[i] - '0' + 'a', 1 );
                if( s == dna && i==N-1 )
                    f = a2;
                arena.makeBond( a, a2 );
                a = a2;
            }
            x += 2;
        }

        if( 1 ) {
            // a loop around the enzymes
            int x = 9;
            int y = 9;
            const size_t a0 = arena.addAtom( x, y, 'a', 1 );
            size_t prev = a0;
            size_t a;
            for(int i = 0; i < vs.size()*2 + 2; i++) {
                x++;
                a = arena.addAtom( x, y, 'a', 1 );
                arena.makeBond(a, prev);
                if( i == 27 )
                    arena.makeBond(a, e);
                prev = a;
            }
            for(int i = 0; i < vs.front().size() + 1; i++) {
                y++;
                a = arena.addAtom( x, y, 'a', 1 );
                arena.makeBond(a, prev);
                prev = a;
            }
            for(int i = 0; i < vs.size()*2 + 2; i++) {
                x--;
                a = arena.addAtom( x, y, 'a', 1 );
                arena.makeBond(a, prev);
                if( i == 17 )
                    arena.makeBond(a, f);
                prev = a;
            }
            for(int i = 0; i < vs.front().size(); i++) {
                y--;
                a = arena.addAtom( x, y, 'a', 1 );
                arena.makeBond(a, prev);
                prev = a;
            }
            arena.makeBond(a, a0);
        }
    }

    if( 1 ) {
        // add some surrounding atoms
        for( int i = 0; i < 500; ++i ) {
            int x = rand() % arena.getArenaWidth();
            int y = rand() % arena.getArenaHeight();
            if( !arena.hasAtom( x, y ) )
                arena.addAtom( x, y, "abcdef"[Arena::getRandIntInclusive(0, 5)], 0 );
        }
    }
}
