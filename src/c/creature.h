#pragma once
/*
 * Ava - the pebble pet
 * @Author: Jan Caspar
 * @Date: 2017-11-08 20:59:40
 * this is the main "pet" implementation
 */

#include <stdbool.h>

/**
 * @brief the 3 main stati of the pet
 * 
 */
typedef enum creatureStatus { Rested, Hapiness, Hunger } CreatureStatus;

/**
 * @brief ADT Creature
 * represents the pet
 * 
 */
typedef struct creature Creature;

/**
 * @brief creates a new creature
 * 
 * @return Creature* pointer of creature
 */
Creature* create_creature();

/**
 * @brief disposes a creature
 * sounds harsh, but memory is expensive.
 * @param c creature to be disposed
 */
void dispose_creature(Creature *c);

/**
 * @brief creature heartbeat
 * this is were the magic happends
 * with every heartbeat the stats change
 * @param c creature on which the heartbeat should be applied
 */
void heartbeat(Creature *c);

/**
 * @brief feed the creature
 * 
 * @param c creature to feed
 * @return true if its feeding
 * @return false if its denying
 */
bool feed(Creature *c);

/**
 * @brief play with the creature
 * 
 * @param c creature to be played with
 * @return true if the creature is playing
 * @return false if the creature denies
 */
bool play(Creature *c);

/**
 * @brief puts the creature to sleep
 * 
 * @param c creature which should be sleeping
 * @return true if sleeping
 * @return false if denied
 */
bool put_to_sleep(Creature *c);

/**
 * @brief checks if the creature is still alive
 * 
 * @param c creature to be checked on
 * @return true alive
 * @return false dead
 */
bool is_alive(Creature *c);

/**
 * @brief checks if the creature is asleep
 * shhhh!
 * 
 * @param c creature to be checked on
 * @return true asleep
 * @return false  awake
 */
bool is_asleep(Creature *c);

/**
 * @brief retrives a status percentage as int
 * possible stati are to be taken from the enum
 * 
 * @param status see enum
 * @param c creature you want the status of
 * @return int percentage as int
 */
int status_percentage(CreatureStatus status, Creature *c);

/**
 * @brief saves the creature to the pebble persistence
 * 
 * @param c creature to be saved
 */
void save_creature(Creature *c);

/**
 * @brief restores a creatures from the pebble persistence
 * 
 * @param hbFrequency the default heartbeat frequence to apply missed heartbeats
 * @return Creature* NULL if there is no creature saved, otherwise the restored creature
 */
Creature* restore_saved_creature(int hbFrequency);