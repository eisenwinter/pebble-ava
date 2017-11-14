#pragma once
/*
 * Ava - the pebble pet
 * @Author: Jan Caspar
 * @Date: 2017-11-12 12:52:01
 * this contains anything related to the animations
 */
#include <stdbool.h>
#include "creature.h"


/**
 * @brief used for the sprite sheet animation
   sprite sheet will be calculated using row major addr within the sheet,
   assuming the used sprite sheets will never have animations spanning more
   than one row (who would do such thing anyways? :P)
 **/
typedef struct spriteAnimation {
    int row;
    int startFrame;
    int endFrame;
} SpriteAnimation;

/**
 * @brief struct using for positioning
   if depth or 3d is wanted a z axis would need to be added and
   the whole thing would need a re-write =p
 */
typedef struct entityPosition {
    bool set;
    int x;
    int y;
} EntityPosition;


/**
 * @brief this is stored in the animation queue to be animated
 * the framesame supplied will be handled by the animation queue
 */
typedef struct animationQueueItem {
    SpriteAnimation animation;
    EntityPosition pos;
    int rep;
    int current_frame;
    int fps_adjust;
} AnimationQueueItem;


/**
 * @brief this will be fire when a certain movevment with a certain animation is wanted
 *
 * @param animation the animation to be done
 * @param pos the position its walking towards to
 */
typedef void animationCallback(AnimationQueueItem *item);

/**
 * @brief spawns a creature on the visible fiel
 *
 * @param c creature to be spawned
 * @param p position where it should be spawned
 * @param cb animation to be done
 */
void add_creature(Creature *c, EntityPosition p, animationCallback *cb);

/**
 * @brief removes the creature from the field
 *
 * @param c creature to be removed
 * @param cb animation to be done
 */
void remove_creature(Creature *c, animationCallback *cb);

/**
 * @brief moves a creature
 *
 * @param c which creature
 * @param p where to
 * @param cb animation to be done
 */
void move_creature(Creature *c, EntityPosition p, animationCallback *cb);


/**
 * @brief lets it yawn
 *
 * @param cb animation to be done
 */
void creature_is_sleepy(animationCallback *cb);

/**
 * @brief hush hush sleepyboi
 *
 * @param cb animation procesor
 */
void creature_falling_alseep(animationCallback *cb);

/**
 * @brief running arrround playing
 *
 * @param c creature
 * @param cb animation to be done
 */
void creature_is_playing(Creature *c, animationCallback *cb);

/**
 * @brief the idle animation used if nothing is on the animation stack
 * this is used when the animation stack is empty, thats why its different 
 * from the rest, it wont take a function pointer as argument but returns the aniamtion item 
 * directly
 * @param c the creature 
 * @return AnimationQueueItem* the creatures idle animation 
 */
AnimationQueueItem* creature_idle_animation(Creature *c);

/**
 * @brief gets the current position of the creature on the screen
 * 
 * @param c creature 
 * @return EntityPosition* position in x and y 
 */
EntityPosition* get_creature_current_position(Creature *c);

/**
 * @brief creature is eating animation
 * 
 * @param cb animation callback
 */
void creature_is_eating(animationCallback *cb);

/**
 * @brief creature is denying something animation
 * 
 * @param cb animation callback
 */
void creature_denying(animationCallback *cb);