#include <pebble.h>
#include "sprite_handler.h"
#include "render_engine.h"
/*
 * @Author: Jan Caspar 
 * @Date: 2017-11-12 13:16:25 
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */


//fps for delta frame time
#define FPS 24

static Layer *_game_area_layer = NULL;

//static BitmapLayer  *s_canvas_layer;
static Layer *_status_layer = NULL;

static AppTimer *_animationRenderTimer = NULL;


static void animationTimerTick(void *data) {

  _animationRenderTimer = app_timer_register(1000/FPS, animationTimerTick, NULL);
 
}


static void render(Layer *layer, GContext *ctx){
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorCyan);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  
  
        
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GBitmap *bg = get_background(); 
  graphics_draw_bitmap_in_rect(ctx,bg ,GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h));
  GBitmap *frame = get_sprite_frame(0,1);   //x y w h
  graphics_draw_bitmap_in_rect(ctx,frame , GRect(5, 5, 32, 32));
}

static void statusbar_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorVividCerulean);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  
  graphics_draw_rect(ctx, GRect(4,4,102,7));
  graphics_draw_rect(ctx, GRect(4,14,102,7));
  graphics_draw_rect(ctx, GRect(4,24,102,7));
  
  graphics_context_set_fill_color(ctx, GColorOrange);
  
  GRect foodBar = GRect(5,5,95,5);
  graphics_fill_rect(ctx,foodBar,0,GCornerNone);
  
  graphics_context_set_fill_color(ctx, GColorOrange);
  
  GRect hapinessBar = GRect(5,15,35,5);
  graphics_fill_rect(ctx,hapinessBar,0,GCornerNone);
  
  GRect restedBar = GRect(5,25,55,5);
  graphics_fill_rect(ctx,restedBar,0,GCornerNone);
}



static void windowUnload(Window *window) {
 layer_destroy(_game_area_layer); 
 layer_destroy(_status_layer);
 dispose_sprites();
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
  
  _animationRenderTimer = app_timer_register(1000/FPS, animationTimerTick, NULL);

}

void initializeRenderEngine(Window *w){
    window_set_window_handlers(w, (WindowHandlers) {
    .load = windowLoad,
    .unload = windowUnload,
  });

}