#pragma once
/*
 * @Author: Jan Caspar 
 * @Date: 2017-11-08 20:59:40 
 * @Last Modified by: Jan Caspar
 * @Last Modified time: 2017-11-08 22:40:14
 */

#include <stdbool.h>

typedef enum age { Baby = 0, Teen = 2, Adult = 6 } Age;

typedef enum creatureStatus { Rested, Hapiness, Hunger } CreatureStatus;

typedef struct creature Creature;


Creature* create_creature();
void dispose_creature(Creature *c);

void heartbeat(Creature *c);

bool feed(Creature *c);

bool play(Creature *c);

bool put_to_sleep(Creature *c);

bool is_alive(Creature *c);

bool is_asleep(Creature *c);

int status_percentage(CreatureStatus status, Creature *c);

void save_creature(Creature *c);

Creature* restore_saved_creature(int hbFrequency);