/*
 * Ava - the pebble pet
 * @Author: Jan Caspar
 * @Date: 2017-11-12 13:16:25
 * this contains anything related to the animations
 */

#include <stdbool.h>
#include <stdlib.h>
#include "creature.h"
#include "animation_helper.h"

//removed asserts as of 2017-11-13 because pebble lib crashes (?)

typedef struct currentPosition {
    EntityPosition position;
    Creature *creature;
} CurrentPosition;

static CurrentPosition *_creatureMap = NULL;
static int _creatureCount = 0;

//was array for the sake of the basic verion ONE CREATURE ONLY
static void initMapArray() {
    _creatureMap = (CurrentPosition *)malloc(sizeof(CurrentPosition));
    _creatureCount = 0;
}

/**
 * @brief gets the creature from the internal map
 * ATTENTION! this one is pretty cheap because I assume its only 1 creature
 * @param c the creature to be found
 */
static CurrentPosition *getCreatureFromMap(Creature *c) {
    return _creatureMap;
}

void add_creature(Creature *c, EntityPosition p, animationCallback *cb) {

    if (_creatureMap == NULL) {
        initMapArray();
    }
    _creatureMap[_creatureCount] = (CurrentPosition) {
        .position = p,
        .creature = c
    };
    _creatureCount++;
}

void remove_creature(Creature *c, animationCallback *cb) {
    free(_creatureMap);
    _creatureCount = 0;
    _creatureMap = NULL;
}

EntityPosition *get_creature_current_position(Creature *c) {
    CurrentPosition *src = getCreatureFromMap(c);
    return &src->position;
}

static AnimationQueueItem *AnimationQueueItemFactory(SpriteAnimation animation, EntityPosition pos, int rep, int fpsAdjust) {
    AnimationQueueItem *item = malloc(sizeof(AnimationQueueItem));
    item->animation = animation;
    item->pos = pos;
    item->rep = rep;
    item->fps_adjust = fpsAdjust;
    item->current_frame = animation.startFrame;
    return item;
}

void creature_is_playing(Creature *c, animationCallback *cb) {
    CurrentPosition *src = getCreatureFromMap(c);
    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 8,
        .startFrame = 0,
        .endFrame = 2
    },
    (EntityPosition) {
        .set = true, .x = 80, .y = src->position.y
    }, 1, 24));
    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 3,
        .startFrame = 0,
        .endFrame = 3
    },
    (EntityPosition) {
        .set = true, .x = 0, .y = src->position.y
    }, 1, 24));
}

void creature_is_eating(animationCallback *cb) {
    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 9,
        .startFrame = 0,
        .endFrame = 3
    },
    (EntityPosition) {
        .set = false
    }, 5, 0));
}

void creature_denying(animationCallback *cb) {

    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 10,
        .startFrame = 0,
        .endFrame = 3
    },
    (EntityPosition) {
        .set = false
    }, 3, 0));
}

void creature_is_sleepy(animationCallback *cb) {
    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 6,
        .startFrame = 0,
        .endFrame = 2
    },
    (EntityPosition) {
        .set = false
    }, 1, 0));
}

void creature_falling_alseep(animationCallback *cb) {
    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 7,
        .startFrame = 0,
        .endFrame = 1
    },
    (EntityPosition) {
        .set = false
    }, 1, 0));

    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 6,
        .startFrame = 0,
        .endFrame = 3
    },
    (EntityPosition) {
        .set = false
    }, 1, 0));

    cb(AnimationQueueItemFactory((SpriteAnimation) {
        .row = 5,
        .startFrame = 0,
        .endFrame = 3
    },
    (EntityPosition) {
        .set = false
    }, 1, 0));
}

void move_creature(Creature *c, EntityPosition p, animationCallback *cb) {
    CurrentPosition *src = getCreatureFromMap(c);

    int deltaY = src->position.y - p.y;
    if (deltaY) {
        if (deltaY < 0) {
            //down
            cb(AnimationQueueItemFactory((SpriteAnimation) {
                .row = 4,
                .startFrame = 0,
                .endFrame = 3
            },
            (EntityPosition) {
                .set = true, .x = p.x, .y = p.y
            }, 1, 24));
        } else {
            //up
            cb(AnimationQueueItemFactory((SpriteAnimation) {
                .row = 2,
                .startFrame = 0,
                .endFrame = 3
            },
            (EntityPosition) {
                .set = true, .x = p.x, .y = p.y
            }, 1, 24));
        }
    }
    int deltaX = src->position.x - p.x;
    if (deltaX) {
        if (deltaX < 0) {
            // right
            cb(AnimationQueueItemFactory((SpriteAnimation) {
                .row = 1,
                .startFrame = 0,
                .endFrame = 3
            },
            (EntityPosition) {
                .set = true, .x = p.x, .y = src->position.y
            }, 1, 24));
        } else {
            //left
            cb(AnimationQueueItemFactory((SpriteAnimation) {
                .row = 3,
                .startFrame = 0,
                .endFrame = 3
            },
            (EntityPosition) {
                .set = true, .x = p.x, .y = src->position.y
            }, 1, 24));
        }
    }
}

AnimationQueueItem *creature_idle_animation(Creature *c) {
    CurrentPosition *src = getCreatureFromMap(c);
    if (is_asleep(src->creature)) {
        return AnimationQueueItemFactory((SpriteAnimation) {
            .row = 7,
            .startFrame = 0,
            .endFrame = 1
        },
        (EntityPosition) {
            .set = false
        }, 1, 0);
    } else {
        if (status_percentage(Rested, src->creature) > 50) {
            return AnimationQueueItemFactory((SpriteAnimation) {
                .row = 0,
                .startFrame = 0,
                .endFrame = 3
            },
            (EntityPosition) {
                .set = false
            }, 1, 0);
        } else {
            return AnimationQueueItemFactory((SpriteAnimation) {
                .row = 6,
                .startFrame = 0,
                .endFrame = 1
            },
            (EntityPosition) {
                .set = false
            }, 1, 0);
        }
    }
}