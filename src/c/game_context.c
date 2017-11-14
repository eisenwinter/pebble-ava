#include <pebble.h>
#include "creature.h"
#include "animation_helper.h"
#include "game_context.h"

#define HEARTBEAT_FREQ 10000
#define MOVE_AFTER_TICKS 2

static AppTimer *_hearbeatTimer = NULL;
static Creature *_creature = NULL;
static animationCallback *_defaultCb = NULL;
static heartbeatCallback *_statiCb = NULL;
static creatureDiedCallback *_deathCb = NULL;
static int _movement_counter = 0;

static void heartbeat_tick(void *data) {
  heartbeat(_creature);
  if(is_alive(_creature)){
     if(!is_asleep(_creature) && status_percentage(Rested,_creature) == 40){
       creature_is_sleepy(_defaultCb);
     }
     if(!is_asleep(_creature)){
       _movement_counter++;
       if(_movement_counter > MOVE_AFTER_TICKS){
         _movement_counter = 0;
         move_creature(_creature,(EntityPosition){ .set = true, .x = rand()%120, .y=rand()%120}, _defaultCb);
       }
     }
    _hearbeatTimer = app_timer_register(HEARTBEAT_FREQ, heartbeat_tick, NULL);
  }else{
    //death message
    _deathCb();
  }
}

static void tryToRestoreCreature(){
  _creature = create_creature("Ava");
}

void init_game_ctx(
                animationCallback *cb, 
                heartbeatCallback *statiCb,
                creatureDiedCallback *deathCb){
  _defaultCb = cb;
  _statiCb = statiCb;
  _deathCb = deathCb;
  tryToRestoreCreature();
  add_creature(_creature,(EntityPosition) { .x = 35, .y = 35},_defaultCb);
  _hearbeatTimer = app_timer_register(HEARTBEAT_FREQ, heartbeat_tick, NULL);
}


void command_play(){
  if(play(_creature)){
    _movement_counter = 0;
    creature_is_playing(_creature, _defaultCb);
  }else{
    creature_denying(_defaultCb);
  }
}
void command_feed(){
  if(feed(_creature)){
    _movement_counter = 0;
    creature_is_eating(_defaultCb);
  }else{
    creature_denying(_defaultCb);
  }
}
void command_put_to_sleep(){
  if((put_to_sleep(_creature))){
    _movement_counter = 0;
    creature_falling_alseep(_defaultCb);
  }else{
    creature_denying(_defaultCb);
  }
  
}

void dispose_game_ctx(){
  app_timer_cancel(_hearbeatTimer);
  remove_creature(_creature,_defaultCb);
  dispose_creature(_creature);
}

Creature* get_creature_from_ctx(){
  return _creature;
}