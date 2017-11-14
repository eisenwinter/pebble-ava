#pragma once
#include <pebble.h>
/*
 * @Author: Jan Caspar
 * @Date: 2017-11-12 13:16:25
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */

 /**
  * @brief initializes the sprite handler
  * 
  */
void init_sprites();

/**
 * @brief gets a ava sprite frame from a certain row
 * 
 * @param row which contains the sprite(s)
 * @param frame the frame to be retrived
 * @return GBitmap* the frame
 */
GBitmap* get_sprite_frame(int row, int frame);

/**
 * @brief gets the background for the game area
 * 
 * @return GBitmap* background image
 */
GBitmap* get_background();

/**
 * @brief gets the legend for the status bars
 * 
 * @return GBitmap* status bars legend
 */
GBitmap* get_status_legend();

/**
 * @brief cleans up all loaded ressources
 * 
 */
void dispose_sprites();