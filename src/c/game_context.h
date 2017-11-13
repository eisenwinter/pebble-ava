#pragma once
#include "animation_helper.h"
typedef void heartbeatCallback();
typedef void creatureDiedCallback();
void command_play();
void command_feed();
void command_put_to_sleep();
void dispose_game_ctx();
void init_game_ctx(
                animationCallback *cb, 
                heartbeatCallback *statiCb,
                creatureDiedCallback *deathCb);
Creature* get_creature_from_ctx();
