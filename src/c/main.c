#include <pebble.h>
#include "game_engine.h"

/*
 * Ava - the pebble pet
 * @Author: Jan Caspar
 * @Date: 2017-11-12 13:16:25
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */

static Window *window = NULL;

static void init(void) {
    window = window_create();
    initialize_game_engine(window);
    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}