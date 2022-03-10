#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "faf_audio.h"
#include "faf_menu.h"
#include "mathlib.h"
#include "sdl_wrapper.h"
#include "window.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif



extern const vector_t FAF_WINDOW_DIMENSIONS;


void faf_on_key(char key, key_event_type_t type, double held_time, window_t *window) {
    assert(window);

    window_on_key(window, key, type, held_time);
}

bool inited = false;
window_t *_window;


void init() {
    sdl_init(VEC_ZERO, FAF_WINDOW_DIMENSIONS);
    sdl_on_key((key_handler_t)faf_on_key);
    srand((unsigned)time(0));

    // Game start also initializes the audio system
    _window = faf_game_start();
}

void c_main(){
    if (!inited) {
        init();
        inited = true;
    }
    double dt = time_since_last_tick();
    window_tick(_window, dt);
    sdl_render_window(_window);
    //faf_audio_play_music();
    if (sdl_is_done(_window)) {
        #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
        #else
        exit(0);
        #endif
        return;
    }
}


int main() {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(c_main, 0, 1);
#else
    while (1) {
        c_main();
    }
#endif
}
