// SDL2:
#include <SDL2/SDL.h>

// Emscripten:
#include <emscripten.h>

// stdlib:
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>

// local:
#include "Arena_SDL.hpp"
#include "base_conversions.hpp"
#include "utils.hpp"

struct context
{
    SDL_Renderer *renderer;
    Arena_SDL arena;
    int iteration;
    float scale;
};

bool is_running = true;

void seed(Arena& arena);

void mainloop(void *arg)
{
    context *ctx = static_cast<context*>(arg);
    SDL_Renderer *renderer = ctx->renderer;

    try {
        if( is_running ) {
            const size_t iterations_per_render = 10;
            for(size_t i = 0; i < iterations_per_render; ++i)
            {
                ctx->arena.update();
            }
            ctx->iteration += iterations_per_render;
        }
        ctx->arena.Draw(renderer, ctx->scale);
    } catch(const std::exception& e) {
        std::cout << "Caught exception in main loop: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Caught unknown exception in main loop" << std::endl;
    }

    SDL_RenderPresent(renderer);
}

extern "C" void EMSCRIPTEN_KEEPALIVE toggle_start_stop() { is_running = !is_running; }

int main()
{
    std::cout << "Initializing..." << std::endl;

    context ctx;

    const float scale = 8.0f;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(ctx.arena.X*scale, ctx.arena.Y*scale, 0, &window, &renderer);
    ctx.renderer = renderer;
    ctx.iteration = 0;
    ctx.scale = scale;

    try {
        seed(ctx.arena);
    } catch(const std::exception& e) {
        std::cout << "Caught exception in initialization: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Caught unknown exception in initialization" << std::endl;
    }

    std::cout << "Running..." << std::endl;
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
    // checks
    const size_t expected_num_digits = ConvertFromMultiBase<Reaction::num_entries>( Reaction::limits, Reaction::limits, Reaction::base ).size();
    if( expected_num_digits != Reaction::num_digits ) {
        std::cout << "Should Reaction::limits be " << expected_num_digits << "?" << std::endl;
        throw std::runtime_error("Reaction::limits mismatch");
    }

    // add a cell
    if( 1 ) {
        // some (random) enzymes and the dna
        size_t max_num_digits = 0;
        std::vector<Reaction::DigitsType> enzymes;
        enzymes.push_back( Reaction( 'a', 1, false, 'i', 0, 2, true, 3 ).getBases() );
        enzymes.push_back( Reaction( 'i', 3, false, 'i', 0, 2, true, 3 ).getBases() );
        for(int i = 0; i < 30; i++) {
            const Reaction r( getRandIntInclusive(0,Reaction::num_types-1)+'a', getRandIntInclusive(0,Reaction::num_states-1),
                getRandIntInclusive(0,1), getRandIntInclusive(0,Reaction::num_types-1)+'a', getRandIntInclusive(0,Reaction::num_states-1),
                getRandIntInclusive(0,Reaction::num_states-1), getRandIntInclusive(0,1), getRandIntInclusive(0,Reaction::num_states-1) );
            enzymes.push_back( r.getBases() );
            const size_t num_digits = ConvertFromMultiBase<Reaction::num_entries>( r.getEntries(), Reaction::limits, Reaction::base ).size();
            std::cout << r.getAsHumanReadableString() << " ( " << num_digits << " )" << std::endl;
            if( num_digits > max_num_digits ) {
                max_num_digits = num_digits;
            }
        }
        std::cout << "Max num digits = " << max_num_digits << std::endl;
        if( max_num_digits > Reaction::num_digits ) {
            std::cout << "Should Reaction::limits be " << max_num_digits << "?" << std::endl;
            throw std::runtime_error("Reaction::limits mismatch");
        }
        std::vector<int> dna;
        dna.push_back(Reaction::base); // start marker
        for(const Reaction::DigitsType& enzyme : enzymes) {
            dna.insert(dna.end(), enzyme.begin(), enzyme.end());
            dna.push_back(Reaction::base+1); // separator
        }
        // draw the dna
        int x = 11;
        int y = Reaction::num_digits + 10;
        const int dna_min_y = y;
        const int dna_max_y = y + Reaction::num_digits*2;
        int dx = 0;
        int dy = 1;
        size_t dna_start, dna_end, prev;
        for(int i = 0; i < dna.size(); i++) {
            const size_t a = arena.addAtom( x, y, dna[i] + 'a', 1 );
            if( i == 0 ) {
                dna_start = a;
            }
            else {
                arena.makeBond(a, prev);
            }
            if( i == dna.size() -1 ) {
                dna_end = a;
            }
            prev = a;
            if(dy > 0 && y == dna_max_y ) {
                // reached bottom, start upwards
                x++;
                dy = -1;
            }
            else if(dy < 0 && y == dna_min_y ) {
                // reached top, start downwards
                x++;
                dy = 1;
            }
            else {
                x += dx;
                y += dy;
            }
        }
        const Atom& dna_start_atom = arena.getAtom(dna_start);
        const Atom& dna_end_atom = arena.getAtom(dna_end);

        if( 1 ) {
            // draw the enzymes above and below (so we get half on each side)
            for(int i = 0; i < enzymes.size(); i++ ) {
                if( i % 2 ) { x = dna_start_atom.x + (i-1)/2; y = dna_min_y - Reaction::num_digits - 1; }
                else { x = dna_start_atom.x + i/2; y = dna_max_y + 2; }
                size_t prev = arena.addAtom( x, y, enzymes[i].front() + 'a', 1);
                for(int j = 1; j < enzymes[i].size(); j++) {
                    const size_t a = arena.addAtom( x, y + j, enzymes[i][j] + 'a', 1);
                    arena.makeBond(a, prev);
                    prev = a;
                }
            }
        }

        if( 1 ) {
            // a loop around the enzymes
            const int min_x = dna_start_atom.x - 1;
            const int min_y = dna_start_atom.y - 1 - Reaction::num_digits - 2;
            const int max_x = dna_end_atom.x + 1;
            const int max_y = dna_max_y + 1 + Reaction::num_digits + 2;
            const size_t a0 = arena.addAtom( min_x, min_y, 'a', 1 );
            size_t prev = a0;
            size_t a;
            for(int x = min_x + 1; x <= max_x; x++) {
                a = arena.addAtom( x, min_y, 'a', 1 );
                arena.makeBond(a, prev);
                prev = a;
            }
            for(int y = min_y + 1; y <= max_y; y++) {
                a = arena.addAtom( max_x, y, 'a', 1 );
                arena.makeBond(a, prev);
                if( y == dna_end_atom.y ) {
                    arena.makeBond( a, dna_end );
                }
                prev = a;
            }
            for(int x = max_x - 1; x >= min_x; x--) {
                a = arena.addAtom( x, max_y, 'a', 1 );
                arena.makeBond(a, prev);
                prev = a;
            }
            for(int y = max_y - 1; y >= min_y; y--) {
                a = arena.addAtom( min_x, y, 'a', 1 );
                arena.makeBond(a, prev);
                if( y == dna_start_atom.y )
                    arena.makeBond(a, dna_start);
                prev = a;
            }
            arena.makeBond(a, a0);
        }
    }

    if( 1 ) {
        // add some surrounding atoms
        for( int i = 0; i < arena.X * arena.Y / 2; ++i ) {
            int x = rand() % arena.getArenaWidth();
            int y = rand() % arena.getArenaHeight();
            if( !arena.hasAtom( x, y ) )
                arena.addAtom( x, y, getRandIntInclusive(0, Reaction::num_types-1) + 'a', 0 );
        }
    }
}
