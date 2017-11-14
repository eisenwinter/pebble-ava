#pragma once
#include <pebble.h>
/*
 * @Author: Jan Caspar 
 * @Date: 2017-11-12 13:16:25 
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */

void init_sprites();
GBitmap* get_sprite_frame(int row, int frame);
GBitmap* get_background();
GBitmap* get_status_legend();
void dispose_sprites();