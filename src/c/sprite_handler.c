#include <pebble.h>
#include "sprite_handler.h"

/*
 * @Author: Jan Caspar
 * @Date: 2017-11-12 13:16:25
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */

#define SPRITE_FRAME_WIDTH 32
#define SPRITE_FRAME_HEIGHT 32

static GBitmap *_basic_sprite_sheet;
static GBitmap *_background;
static GBitmap *_legend;

static GBitmap *_sub = NULL;

void init_sprites() {
    _basic_sprite_sheet = gbitmap_create_with_resource(RESOURCE_ID_AVA_SPRITES);
    _background = gbitmap_create_with_resource(RESOURCE_ID_INDOOR_BACKGROUND);
    _legend = gbitmap_create_with_resource(RESOURCE_ID_STATUS_BAR);
}

static GRect get_sprite_frame_rectangle(int row, int frame) {
    return GRect(frame*SPRITE_FRAME_WIDTH,row*SPRITE_FRAME_HEIGHT,SPRITE_FRAME_WIDTH,SPRITE_FRAME_HEIGHT);
}

GBitmap* get_sprite_frame(int row, int frame) {
    //according to the pebble doc this will only create a link to the main image
    //so we willl just finish of the main image in the end
    //redo: this proved to be wrong; shame on the docu, so we dispose the sub image if set
    if(_sub != NULL) {
        gbitmap_destroy(_sub);
    }
    _sub = gbitmap_create_as_sub_bitmap(_basic_sprite_sheet,get_sprite_frame_rectangle(row,frame));
    return _sub;
}

GBitmap* get_background() {
    return _background;
}

GBitmap* get_status_legend() {
    return _legend;
}

void dispose_sprites() {
    gbitmap_destroy(_basic_sprite_sheet);
    gbitmap_destroy(_background);
    gbitmap_destroy(_legend);
}