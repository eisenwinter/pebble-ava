#include <pebble.h>
#include "creature.h"
#include "animation_helper.h"
#include "game_context.h"

#define HEARTBEAT_FREQ 10000
#define MOVE_AFTER_TICKS 2

static AppTimer *_hearbeat_timer = NULL;
static Creature *_creature = NULL;
static animationCallback *_default_callback = NULL;
static heartbeatCallback *_stati_callback = NULL;
static creatureDiedCallback *_death_callback = NULL;
static int _movement_counter = 0;

static void heartbeat_tick(void *data) {
    heartbeat(_creature);
    if(is_alive(_creature)) {
        if(!is_asleep(_creature) && status_percentage(Rested,_creature) == 40) {
            creature_is_sleepy(_default_callback);
        }
        if(!is_asleep(_creature)) {
            _movement_counter++;
            if(_movement_counter > MOVE_AFTER_TICKS) {
                _movement_counter = 0;
                //desk starts at 127 y
                move_creature(_creature,(EntityPosition) {
                    .set = true, .x = rand()%100, .y=rand()%110
                }, _default_callback);
            }
        }
        _hearbeat_timer = app_timer_register(HEARTBEAT_FREQ, heartbeat_tick, NULL);
    } else {
        //death message
        _death_callback();
        if(persist_exists(101)) {
             for(int i = 101; i <= 108; i++) {
                 persist_delete(i);
             }
             persist_delete(50);
             persist_delete(60);
         }
    }
}

static void try_to_restore_creature() {
    _creature = restore_saved_creature(HEARTBEAT_FREQ);
    if(_creature != NULL) {
        int x = persist_read_int(50);
        int y = persist_read_int(60);
        EntityPosition restored_pos = { .x = x, .y = y };
        add_creature(_creature,restored_pos,_default_callback);

    } else {
        _creature = create_creature();
        add_creature(_creature,(EntityPosition) {
            .x = 35, .y = 35
        },_default_callback);
    }
}

static void try_store_creature() {
    if(is_alive(_creature)) {
        save_creature(_creature);
        persist_write_int(50,get_creature_current_position(_creature)->x);
        persist_write_int(60,get_creature_current_position(_creature)->y);
    } else {
        if(persist_exists(101)) {
            for(int i = 101; i <= 108; i++) {
                persist_delete(i);
            }
            persist_delete(50);
            persist_delete(60);
        }

    }
}

void init_game_ctx(
    animationCallback *cb,
    heartbeatCallback *statiCb,
    creatureDiedCallback *deathCb) {
    _default_callback = cb;
    _stati_callback = statiCb;
    _death_callback = deathCb;
    try_to_restore_creature();
    _hearbeat_timer = app_timer_register(HEARTBEAT_FREQ, heartbeat_tick, NULL);
}


void command_play() {
    if(play(_creature)) {
        _movement_counter = 0;
        creature_is_playing(_creature, _default_callback);
    } else {
        if(!is_asleep(_creature)) {
            creature_denying(_default_callback);
        }
    }
}
void command_feed() {
    if(feed(_creature)) {
        _movement_counter = 0;
        creature_is_eating(_default_callback);
    } else {
        if(!is_asleep(_creature)) {
            creature_denying(_default_callback);
        }
    }
}
void command_put_to_sleep() {
    if((put_to_sleep(_creature))) {
        _movement_counter = 0;
        creature_falling_alseep(_default_callback);
    } else {
        if(!is_asleep(_creature)) {
            creature_denying(_default_callback);
        }
    }
}

void dispose_game_ctx() {
    app_timer_cancel(_hearbeat_timer);
    try_store_creature();
    remove_creature(_creature,_default_callback);
    dispose_creature(_creature);
    _creature = NULL;
}

Creature* get_creature_from_ctx() {
    return _creature;
}