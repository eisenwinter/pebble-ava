#pragma once
/*
 * @Author: Jan Caspar 
 * @Date: 2017-11-12 12:52:32 
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 13:49:28
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
     int x;
     int y;
} EntityPosition;

/**
 * @brief this will be fire when a certain movevment with a certain animation is wanted
 * 
 * @param animation the animation to be done
 * @param pos the position its walking towards to 
 */
typedef void animationCallback(SpriteAnimation animation, EntityPosition pos, int speed);
 
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
  * @param c creature
  * @param cb animation to be done
  */
 void creature_is_sleepy(Creature *c, animationCallback *cb);

/**
 * @brief hush hush sleepyboi
 * 
 * @param c creature thats falling asleep
 * @param cb animation procesor
 */
 void creature_falling_alseep(Creature *c, animationCallback *cb);

 /**
  * @brief running arrround playing
  * 
  * @param c creature
  * @param cb animation to be done
  */
 void creature_is_playing(Creature *c, animationCallback *cb);