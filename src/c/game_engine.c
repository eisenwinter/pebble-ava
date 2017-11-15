#include <pebble.h>
#include <stdbool.h>
#include "sprite_handler.h"
#include "game_engine.h"
#include "animation_helper.h"
#include "creature.h"
#include "game_context.h"
/*
 * @Author: Jan Caspar
 * @Date: 2017-11-12 13:16:25
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */

//how many animations we can be behind while rendering
#define MAX_ANIMATION_QUEUE 32
//fps for delta frame time
#define DEFAULT_FPS 3


static Layer *_window_layer = NULL;
static Layer *_game_area_layer = NULL;
static Layer *_status_layer = NULL;
static TextLayer* _msg_box_layer = NULL;
static AppTimer *_animation_render_timer = NULL;
static Creature *_current_creature = NULL;
static int current_fps = DEFAULT_FPS;

static AnimationQueueItem *_animation_queue[MAX_ANIMATION_QUEUE];
static int _animation_queue_size = 0;
static AnimationQueueItem *_idle_animation = NULL;

/**
 * @brief adds a new item the queue
 * if the queue is full the item will be disposed
 * @param item the item to be added
 */
static void add_to_animation_queue(AnimationQueueItem *item) {
    if(_animation_queue_size < MAX_ANIMATION_QUEUE) {
        _animation_queue[_animation_queue_size] = item;
        _animation_queue_size++;
    } else {
        free(item);
    }
}

static void animation_timer_tick(void *data) {
    layer_mark_dirty(_game_area_layer);
    _animation_render_timer = app_timer_register(1000/current_fps, animation_timer_tick, NULL);

}

static void update_stati() {
    layer_mark_dirty(_status_layer);
}


static AnimationQueueItem* dequeueAnimation() {
    return _animation_queue[_animation_queue_size-1];
}

/**
 * @brief this takes care of the main area of the game
 * its drawing everything except for the status bars area and the death message
 */
static void render(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    graphics_context_set_fill_color(ctx, GColorCyan);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);

    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    GBitmap *bg = get_background();
    graphics_draw_bitmap_in_rect(ctx,bg,GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h));

    if(_animation_queue_size > 0) {
        free(_idle_animation);
        _idle_animation = NULL;
        AnimationQueueItem *item = dequeueAnimation();
        current_fps = (item->fps_adjust) ? item->fps_adjust : DEFAULT_FPS;
        GBitmap *frame = get_sprite_frame(item->animation.row,item->current_frame);
        EntityPosition *p = get_creature_current_position(_current_creature);
        graphics_draw_bitmap_in_rect(ctx,frame, GRect(p->x, p->y, 32, 32));
        item->current_frame++;

        int deltaX = (item->pos.set) ? p->x - item->pos.x : 0;
        int deltaY = (item->pos.set) ? p->y - item->pos.y : 0;
        if(deltaX || deltaY) {
            if(item->current_frame > item->animation.endFrame) {
                item->current_frame = item->animation.startFrame;
            }
        }
        if(deltaX) {
            if(deltaX < 0) {
                p->x++;
            } else {
                p->x--;
            }
        } else if(deltaY) {
            if(deltaY < 0) {
                p->y++;
            } else {
                p->y--;
            }
        } else {
            if(item->rep > 1) {
                item->rep--;
            } else {
                if(item->current_frame > item->animation.endFrame) {
                    free(item);
                    _animation_queue[_animation_queue_size-1] = NULL;
                    _animation_queue_size--;
                }
            }
        }
    } else {
        if(_idle_animation == NULL) {
            _idle_animation = creature_idle_animation(_current_creature);
            if(current_fps != DEFAULT_FPS) { //restore default fps
               current_fps = DEFAULT_FPS;
            }
        }

        GBitmap *frame = get_sprite_frame(_idle_animation->animation.row,_idle_animation->current_frame);
        EntityPosition *p = get_creature_current_position(_current_creature);
        graphics_draw_bitmap_in_rect(ctx,frame, GRect(p->x, p->y, 32, 32));
        _idle_animation->current_frame++;
        if(_idle_animation->current_frame > _idle_animation->animation.endFrame) {
            _idle_animation->current_frame = _idle_animation->animation.startFrame;
        }

    }
}

/**
 * @brief this handles the status bar drawing
 */
static void statusbar_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    graphics_context_set_fill_color(ctx,PBL_IF_COLOR_ELSE(GColorVividCerulean, GColorWhite));
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);

    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    GBitmap *statuslegend = get_status_legend();
    graphics_draw_bitmap_in_rect(ctx,statuslegend,GRect(bounds.origin.x + 111, bounds.origin.y + 5, 30, 25));

    graphics_draw_rect(ctx, GRect(4,4,102,7));
    graphics_draw_rect(ctx, GRect(4,14,102,7));
    graphics_draw_rect(ctx, GRect(4,24,102,7));

    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorOrange, GColorBlack));

    int foodPercs = status_percentage(Hunger,_current_creature);
    GRect foodBar = GRect(5,5,foodPercs,5);
    graphics_fill_rect(ctx,foodBar,0,GCornerNone);

    int hapPercs = status_percentage(Hapiness,_current_creature);
    GRect hapinessBar = GRect(5,15,hapPercs,5);
    graphics_fill_rect(ctx,hapinessBar,0,GCornerNone);

    int restedPercs = status_percentage(Rested,_current_creature);
    GRect restedBar = GRect(5,25,restedPercs,5);
    graphics_fill_rect(ctx,restedBar,0,GCornerNone);

    graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorCobaltBlue,GColorBlack));
    graphics_draw_line(ctx, GPoint(0,bounds.size.h-1), GPoint(bounds.size.w,bounds.size.h-1));
}

static void creature_died() {
    app_timer_cancel(_animation_render_timer);
    GRect bounds = layer_get_bounds(_game_area_layer);
    _msg_box_layer = text_layer_create(GRect(bounds.origin.x+5, bounds.origin.y+25, bounds.size.w-10, bounds.size.h-15));
    text_layer_set_text_alignment(_msg_box_layer, GTextAlignmentCenter);
    text_layer_set_text(_msg_box_layer,"“Like a flame burning away the darkness, life is flesh on bone convulsing above the ground.”\nE. Elias Merhige\n\nAva has died.");
    text_layer_set_background_color(_msg_box_layer,GColorBlack);
    text_layer_set_text_color(_msg_box_layer,GColorWhite);

    layer_add_child(_window_layer, text_layer_get_layer(_msg_box_layer));
}

static void clear_animation_queue() {
    while(_animation_queue_size > 0) {
        free(_animation_queue[_animation_queue_size-1]);
        _animation_queue[_animation_queue_size-1] = NULL;
        _animation_queue_size--;
    }
}

static void restart() {
    layer_destroy(text_layer_get_layer(_msg_box_layer));
    _msg_box_layer = NULL;
    dispose_game_ctx();
    clear_animation_queue();
    init_game_ctx(add_to_animation_queue,update_stati,creature_died);
    _current_creature = get_creature_from_ctx();
    _animation_render_timer = app_timer_register(1000/DEFAULT_FPS, animation_timer_tick, NULL);
}

static bool controls_enabled() {
    if(_current_creature != NULL && !is_alive(_current_creature)) {
        restart();
        return false;
    }
    return _animation_queue_size <= 1;
}


static void windowUnload(Window *window) {
    app_timer_cancel(_animation_render_timer);
    clear_animation_queue();
    layer_destroy(_game_area_layer);
    layer_destroy(_status_layer);
    dispose_game_ctx();
    dispose_sprites();
    light_enable(false);
}

static void windowLoad(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    init_sprites();

    //layer to draw the status bars on
    _status_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 35));
    layer_add_child(window_layer, _status_layer);
    layer_set_update_proc(_status_layer, statusbar_update_proc);


    _game_area_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y+35, bounds.size.w, bounds.size.h-35));
    layer_set_update_proc(_game_area_layer, render);
    layer_add_child(window_layer, _game_area_layer);

    _animation_render_timer = app_timer_register(1000/DEFAULT_FPS, animation_timer_tick, NULL);
    _window_layer = window_layer;
    light_enable(true);

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(controls_enabled()) {
        command_play();
    }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(controls_enabled()) {
        command_feed();
    }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(controls_enabled()) {
        command_put_to_sleep();
    }

}


static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void initialize_game_engine(Window *w) {
    init_game_ctx(add_to_animation_queue,update_stati,creature_died);
    _current_creature = get_creature_from_ctx();
    window_set_window_handlers(w, (WindowHandlers) {
        .load = windowLoad,
         .unload = windowUnload,
    });
    window_set_click_config_provider(w, click_config_provider);

}