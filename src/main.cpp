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
    Neighborhood bond_range = Neighborhood::Moore;

    // an 8-cell loop with some rigid sections
    if( 0 ) {
        size_t a = arena.addAtom( 1, 1, 0 );
        size_t b = arena.addAtom( 2, 1, 0 );
        size_t c = arena.addAtom( 2, 2, 0 );
        size_t d = arena.addAtom( 1, 2, 0 );
        size_t e = arena.addAtom( 1, 3, 0 );
        size_t f = arena.addAtom( 0, 3, 0 );
        size_t g = arena.addAtom( 0, 2, 0 );
        size_t h = arena.addAtom( 0, 1, 0 );
        arena.makeBond( a, b, Neighborhood::vonNeumann );
        arena.makeBond( b, c, Neighborhood::vonNeumann );
        arena.makeBond( c, d, Neighborhood::Moore );
        arena.makeBond( d, e, Neighborhood::Moore );
        arena.makeBond( e, f, Neighborhood::vonNeumann );
        arena.makeBond( f, g, Neighborhood::Moore );
        arena.makeBond( g, h, Neighborhood::vonNeumann );
        arena.makeBond( h, a, Neighborhood::Moore );
    }

    // a box with flailing arms
    if( 0 ) {
        size_t a = arena.addAtom( 10, 10, 1 );
        size_t b = arena.addAtom( 11, 10, 1 );
        size_t c = arena.addAtom( 12, 10, 1 );
        size_t d = arena.addAtom( 12, 11, 1 );
        size_t e = arena.addAtom( 11, 11, 1 );
        size_t f = arena.addAtom( 10, 11, 1 );
        size_t g = arena.addAtom( 10, 12, 1 );
        size_t h = arena.addAtom( 11, 12, 1 );
        size_t i = arena.addAtom( 12, 12, 1 );
        size_t j = arena.addAtom( 9, 9, 1 );
        size_t k = arena.addAtom( 8, 8, 1 );
        size_t l = arena.addAtom( 7, 7, 1 );
        size_t m = arena.addAtom( 11, 9, 1 );
        size_t n = arena.addAtom( 12, 8, 1 );
        size_t o = arena.addAtom( 13, 7, 1 );
        arena.makeBond( a, b, Neighborhood::vonNeumann );
        arena.makeBond( b, c, Neighborhood::vonNeumann );
        arena.makeBond( c, d, Neighborhood::vonNeumann );
        arena.makeBond( d, e, Neighborhood::vonNeumann );
        arena.makeBond( e, f, Neighborhood::vonNeumann );
        arena.makeBond( f, g, Neighborhood::vonNeumann );
        arena.makeBond( g, h, Neighborhood::vonNeumann );
        arena.makeBond( h, i, Neighborhood::vonNeumann );
        arena.makeBond( a, j, Neighborhood::Moore );
        arena.makeBond( j, k, Neighborhood::Moore );
        arena.makeBond( k, l, Neighborhood::Moore );
        arena.makeBond( c, m, Neighborhood::Moore );
        arena.makeBond( m, n, Neighborhood::Moore );
        arena.makeBond( n, o, Neighborhood::Moore );
    }

    // a double-stranded molecule
    if( 0 ) {
        size_t a = arena.addAtom( 21, 21, 5 );
        size_t b = arena.addAtom( 22, 21, 3 );
        size_t c = arena.addAtom( 21, 22, 5 );
        size_t d = arena.addAtom( 22, 22, 3 );
        size_t e = arena.addAtom( 21, 23, 5 );
        size_t f = arena.addAtom( 22, 23, 3 );
        size_t g = arena.addAtom( 21, 24, 5 );
        size_t h = arena.addAtom( 22, 24, 3 );
        size_t i = arena.addAtom( 21, 25, 5 );
        size_t j = arena.addAtom( 22, 25, 3 );
        size_t k = arena.addAtom( 21, 26, 5 );
        size_t l = arena.addAtom( 22, 26, 3 );
        arena.makeBond( a, b, bond_range );
        arena.makeBond( c, d, bond_range );
        arena.makeBond( e, f, bond_range );
        arena.makeBond( g, h, bond_range );
        arena.makeBond( i, j, bond_range );
        arena.makeBond( k, l, bond_range );
        arena.makeBond( a, c, bond_range );
        arena.makeBond( b, d, bond_range );
        arena.makeBond( c, e, bond_range );
        arena.makeBond( d, f, bond_range );
        arena.makeBond( e, g, bond_range );
        arena.makeBond( f, h, bond_range );
        arena.makeBond( g, i, bond_range );
        arena.makeBond( h, j, bond_range );
        arena.makeBond( i, k, bond_range );
        arena.makeBond( j, l, bond_range );
    }

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
        vs.push_back( "210103" );
        vs.push_back( Reaction( 12, true, 10, 6, true, 5 ).getString() );
        vs.push_back( Reaction( 13, true, 10, 7, true, 5 ).getString() );
        vs.push_back( Reaction( 2, true, 11, 2, true, 14 ).getString() );
        vs.push_back( Reaction( 14, true, 6, 0, true, 14 ).getString() );
        vs.push_back( Reaction( 14, true, 7, 1, true, 14 ).getString() );
        vs.push_back( Reaction( 14, true, 8, 3, false, 15 ).getString() );
        vs.push_back( Reaction( 15, false, 4, 14, true, 3 ).getString() );
        vs.push_back( Reaction( 14, true, 10, 2, false, 9 ).getString() );
        int x = 11;
        for( const std::string& s : vs ) {
            const int N = s.length();
            int y = 10;
            size_t a = arena.addAtom( x, y, s[0] - '0' );
            for( int i = 1; i < N; ++i ) {
                y++;
                size_t a2 = arena.addAtom( x, y, s[i] - '0' );
                arena.makeBond( a, a2, bond_range );
                a = a2;
            }
            x += 2;
        }
    }

    if( 1 ) {
        // add some surrounding atoms
        for( int i = 0; i < 500; ++i ) {
            int x = rand() % arena.getArenaWidth();
            int y = rand() % arena.getArenaHeight();
            if( !arena.hasAtom( x, y ) )
                arena.addAtom( x, y, 4 );
        }
    }
}
