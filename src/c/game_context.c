#include <pebble.h>
#include "creature.h"
#include "animation_helper.h"
#include "game_context.h"

#define HEARTBEAT_FREQ 100000

static AppTimer *_hearbeatTimer = NULL;
static Creature *_creature = NULL;
static animationCallback *_defaultCb = NULL;
static heartbeatCallback *_statiCb = NULL;
static creatureDiedCallback *_deathCb = NULL;

static void heartbeat_tick(void *data) {
  heartbeat(_creature);
  if(is_alive(_creature)){
     if(!is_asleep(_creature) && status_percentage(Rested,_creature) == 40){
       creature_is_sleepy(_creature, _defaultCb);
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
    creature_is_playing(_creature, _defaultCb);
  }
}
void command_feed(){
  if(feed(_creature)){
    //missing animation
  }
}
void command_put_to_sleep(){
  if((put_to_sleep(_creature))){
    creature_falling_alseep(_creature, _defaultCb);
  } //might wanna add a deny animation
  
}

void dispose_game_ctx(){
  app_timer_cancel(_hearbeatTimer);
  dispose_creature(_creature);
}

Creature* get_creature_from_ctx(){
  return _creature;
}