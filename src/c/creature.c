 //timeh.h not working?!
#include <pebble.h>
#include <stdlib.h>
#include "creature.h"
#define DEFAULT_STAT 50
/*
 * @Author: Jan Caspar 
 * @Date: 2017-11-08 20:59:40 
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-08 22:40:14
 */ 

// note - default stat isnt the maximum stat that can be reached, its a mere optimum that should be reached
// on the UI anything above the optimum will apear as 100 % but will decrease slower 
// depending on how far I get there might also be negative sideffects if you reach too far beyond the optimum
#define DEFAULT_FOOD_VALUE 25
#define DEFAULT_PLAY_VALUE 12

typedef struct creature {
    time_t born;
    time_t last_save;
	  Age currentAge;
    bool asleep;
    int hunger;
    int hapiness;
    int rested;
    int health;
} Creature;

Creature* create_creature(){
	Creature *c = malloc(sizeof(Creature));
	c->hunger = DEFAULT_STAT / 2;
	c->hapiness = DEFAULT_STAT / 2;
	c->rested = DEFAULT_STAT / 2;
	c->currentAge = Baby;
	c->health = DEFAULT_STAT;
	c->born = time(0);
	c->asleep = false;
	return c;
}



void dispose_creature(Creature *c){
	free(c);
}

static void isAsleepRoutine(Creature *c){
	if(c->rested >= DEFAULT_STAT){
			c->asleep = false;
	}else{
		if(c->rested < DEFAULT_STAT){
			c->rested++;
		}
		if(c->hapiness > 0){
			c->hapiness--;
		}
	}
}

static void isAwakeRoutine(Creature *c){
	if(c->hunger > 0){
		c->hunger--;
	}
	if(c->rested > 0){
		c->rested--;
	}
	if(c->hapiness > 0){
		c->hapiness--;
	}
	
}

static void setHealth(Creature *c){
	//its healthy when its not hungry, not unhappy and rested - so we simply
	//assume all 3 factor the same into the health level (for now, might wanna check up 
	//on how the original thing handled this) 
	c->health = (((c->hunger + c->hapiness + c->rested) / 3) + c->health) / 2;
}

static void updateStati(Creature *c){
	if(c->asleep){
		isAsleepRoutine(c);
	}else{
		isAwakeRoutine(c);
	}
	setHealth(c);
}

void heartbeat(Creature *c){
	updateStati(c);
}

bool feed(Creature *c){
  if(c->asleep){
    return false;
  }
	if(c->hunger < DEFAULT_STAT){
		//"overfeeding" is possible by design
		c->hunger = c->hunger + (DEFAULT_FOOD_VALUE - c->currentAge);
		return true;
	}
	return false;
}

bool play(Creature *c){
  if(c->asleep){
    return false;
  }
	if(c->hapiness < DEFAULT_STAT){
		//but is there really a boundry for hapiness?!
		//well same as overfeeding, it can be "overhappy" as well
		c->hapiness = c->hapiness + (DEFAULT_FOOD_VALUE - c->currentAge);
		return true;
	}
	return false;
}
 
bool put_to_sleep(Creature *c){
	if((!c->asleep) && (c->rested < DEFAULT_STAT*0.41)){
		c->asleep = true;
		return true;
	}
	return false;
		
}

bool is_alive(Creature *c){
	return c->health >= DEFAULT_STAT*0.12; //3 main stats = 33%
}

bool is_asleep(Creature *c){
	return c->asleep;
}

int status_percentage(CreatureStatus status, Creature *c){
  if(!is_alive(c)){
    return 0;
  }
	switch(status){
		case Rested: ; //variable decl is no statement - so empty statement fix :/
			int r = (c->rested * 100) / DEFAULT_STAT;
			if(r > 100){ //"overrested"
				r = 100;
			}
			return r;
			break;
		case Hapiness: ;
			int h = (c->hapiness * 100) / DEFAULT_STAT;
			if(h > 100){ //"overhappy"
				h = 100;
			}
			return h;
		case Hunger: ; 
			int f = (c->hunger * 100) / DEFAULT_STAT;
			if(f > 100){ //"overfed"
				f = 100;
			}
			return f;
		default: 
			return -1;
	}
}

void save_creature(Creature *c){
   persist_write_int(101,c->hapiness);
   persist_write_int(102,c->rested);
   persist_write_int(103,c->hunger);
   persist_write_int(104,c->health);
   persist_write_int(105,c->currentAge);
   persist_write_bool(106,c->asleep);
   persist_write_data(107,&c->born,sizeof(time_t));
   c->last_save = time(0);
   persist_write_data(108,&c->last_save,sizeof(time_t));
}

Creature* restore_saved_creature(int hbFrequency){
   if(persist_exists(101)){
     Creature *c = malloc(sizeof(Creature));
     c->hapiness = persist_read_int(101);
     c->rested = persist_read_int(102);
     c->hunger = persist_read_int(103);
     c->health = persist_read_int(104);
     c->currentAge = persist_read_int(105);
     c->asleep = persist_read_bool(106); 
     persist_read_data(107, &c->born,sizeof(time_t));
     int hbsSecs = hbFrequency / 1000;
     //time passes slower when no on screen
     int delta = ((time(0) - c->last_save) / hbsSecs) / 10;
     for(int i = 0; i < delta; i++){
       heartbeat(c);
     }
     //ava only ages when you  dont look, its like in real life - you certainly wont
     //notice people aging you see every day but you rather notice when you havent seen
     //them in a while
     int ageDelta = (time(0) - c->last_save) / hbsSecs;
     if(ageDelta > 100 && c->currentAge == Baby){
       c->currentAge = Teen;
     }else if(ageDelta > 300 && c->currentAge == Teen){
       c->currentAge = Adult;
     }
     return c;
   }
  return NULL;
}
