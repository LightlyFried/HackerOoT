#ifndef Z_EM_TITLE_CARD_H
#define Z_EM_TITLE_CARD_H

#include "ultra64.h"
#include "actor.h"


struct EmTitleCard;

typedef void (*EmTitleCardActionFunc)(struct EmTitleCard*, struct PlayState*);

typedef struct EmTitleCardTextureContext {
    void* texture;
    s16 x;
    s16 y;
    u8 width;
    u8 height;
    s16 alpha;
    s16 intensity;
} EmTitleCardTextureContext;

typedef struct EmTitleCard {
    Actor actor;
    EmTitleCardActionFunc actionFunc;
    EmTitleCardTextureContext titleTextureCtx;
    EmTitleCardTextureContext subtitleTextureCtx;
    u8 stepDuration; // Frames remaining in this animation step before continuing to the next
} EmTitleCard;

#endif