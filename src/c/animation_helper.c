#include <stdbool.h>
#include <stdlib.h>
#include "creature.h"
#include "animation_helper.h"
/*
 * @Author: Jan Caspar 
 * @Date: 2017-11-12 13:16:25 
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-12 14:53:59
 */


//removed asserts as of 2017-11-13 because pebble lib crashes (?)

#define X_BOUND_UPPPER_LIMIT 135
#define X_BOUND_LOWER_LIMIT 0
#define Y_BOUND_UPPER_LIMIT 135
#define Y_BOUND_LOWER_LIMIT 35
//altough for the basic version theres only 1 creature
//one might want to extend it later - check out ATTENTION! tags in comments
//down below what would need to be fixed for this to work
#define MAX_CREATURES_ON_SCREEN 1


typedef struct currentPosition {
    EntityPosition position;
    Creature *creature;
} CurrentPosition;
    

static CurrentPosition* _creatureMap = NULL;
static int _creatureCount = 0;

static void initMapArray(){
    _creatureMap = (CurrentPosition*)malloc(sizeof(_creatureMap)*MAX_CREATURES_ON_SCREEN);
    _creatureCount = 0;
}

/**
 * @brief gets the creature from the internal map
 * ATTENTION! this one is pretty cheap because I assume its only 1 creature,
 * for multi-creature support this needs to be fixed up!
 * @param c the creature to be found
 */
static CurrentPosition getCreatureFromMap(Creature *c){


    return _creatureMap[0];
}

void add_creature(Creature *c, EntityPosition p, animationCallback *cb){

    if(_creatureMap == NULL){
        initMapArray();
    }
    _creatureMap[_creatureCount] = (CurrentPosition) {
        .position = p,
        .creature = c
    };
    _creatureCount++;
}


void remove_creature(Creature *c, animationCallback *cb){

    //again - for now one creature so this would need to be fixed up for more creatures
    free(_creatureMap);
    _creatureCount = 0;
    _creatureMap = NULL;
}

void creature_is_playing(Creature *c, animationCallback *cb){
     CurrentPosition src = getCreatureFromMap(c);
     cb((SpriteAnimation){
        .row = 8,
        .startFrame = 0,
        .endFrame = 2
     },(EntityPosition){
        .x = src.position.x,
        .y = src.position.y
    },1);
}


void creature_is_sleepy(Creature *c, animationCallback *cb){
     CurrentPosition src = getCreatureFromMap(c);
     cb((SpriteAnimation){
        .row = 6,
        .startFrame = 0,
        .endFrame = 3
     },(EntityPosition){
        .x = src.position.x,
        .y = src.position.y
    },1);
}

void creature_falling_alseep(Creature *c, animationCallback *cb){
    CurrentPosition src = getCreatureFromMap(c);
    cb((SpriteAnimation){
        .row = 5,
        .startFrame = 0,
        .endFrame = 3
     },(EntityPosition){
        .x = src.position.x,
        .y = src.position.y
    },1);
    cb((SpriteAnimation){
        .row = 6,
        .startFrame = 0,
        .endFrame = 3
     },(EntityPosition){
        .x = src.position.x,
        .y = src.position.y
    },1);
     cb((SpriteAnimation){
        .row = 7,
        .startFrame = 0,
        .endFrame = 1
     },(EntityPosition){
        .x = src.position.x,
        .y = src.position.y
     },1);
}

void move_creature(Creature *c, EntityPosition p, animationCallback *cb){

    CurrentPosition src = getCreatureFromMap(c);
    int deltaX = src.position.x - p.x;
    if(deltaX){
        if(deltaX < 0){ // right
            cb((SpriteAnimation){
                .row = 1,
                .startFrame = 0,
                .endFrame = 3
            },(EntityPosition){
                .x = p.x,
                .y = src.position.y
            },1);
        }else{ //left
             cb((SpriteAnimation){
                .row = 3,
                .startFrame = 0,
                .endFrame = 3
            },(EntityPosition){
                .x = p.x,
                .y = src.position.y
            },1);
        }
    }
    int deltaY = src.position.y - p.y;
    if(deltaY){
        if(deltaY < 0){ //down
            cb((SpriteAnimation){
                .row = 4,
                .startFrame = 0,
                .endFrame = 3
            },(EntityPosition){
                .x = p.x,
                .y = p.y
            },1);
        }else{ //up
            cb((SpriteAnimation){
                .row = 2,
                .startFrame = 0,
                .endFrame = 3
            },(EntityPosition){
                .x = p.x,
                .y = p.y
            },1);
        }
    }
}