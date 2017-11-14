#pragma once
#include "animation_helper.h"

/*
 * Ava - the pebble pet
 * @Author: Jan Caspar
 * @Date: 2017-11-12 13:16:25
 * this containts the main game handling
 */

/**
 * @brief function pointer for heartbeat callbacks
 * 
 */
typedef void heartbeatCallback();

/**
 * @brief function pointer for death callback
 * 
 */
typedef void creatureDiedCallback();

/**
 * @brief try to play with creature
 * 
 */
void command_play();

/**
 * @brief try to feed creature
 * 
 */
void command_feed();

/**
 * @brief try to put creature to sleep
 * 
 */
void command_put_to_sleep();

/**
 * @brief dispose the active context
 * 
 */
void dispose_game_ctx();

/**
 * @brief initialize the context
 * 
 * @param cb 
 * @param statiCb 
 * @param deathCb 
 */
void init_game_ctx(
    animationCallback *cb,
    heartbeatCallback *statiCb,
    creatureDiedCallback *deathCb);
    
/**
 * @brief retrive the creature currently used in the context
 * 
 * @return Creature* active creature
 */
Creature* get_creature_from_ctx();
