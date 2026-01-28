#ifndef Z_EM_TITLE_CARD_H
#define Z_EM_TITLE_CARD_H

#include "ultra64.h"
#include "actor.h"


struct EmTitleCard;

typedef void (*EmTitleCardActionFunc)(struct EmTitleCard*, struct PlayState*);

typedef struct EmTitleCardTextureContext {
    void* texture;
    s16 x1;
    s16 x2;
    s16 y1;
    s16 y2;
    s16 width;
    s16 height;
} EmTitleCardTextureContext;

typedef struct EmTitleCard {
    Actor actor;
    s16 alpha; // Alpha and intensity *should* be u8s, but `Math_StepToS` expects a s16
    s16 intensity;
    EmTitleCardActionFunc actionFunc;
    EmTitleCardTextureContext titleTextureCtx;
    EmTitleCardTextureContext dividerTextureCtx;
    EmTitleCardTextureContext subtitleTextureCtx;
    u8 keyframeCountdown; // Frames remaining in this animation step before continuing to the next
    s16 subtitleCurrentHeight; // Controls the sliding animation for the subtitle (separate from EmTitleCardTextureContext.height so we keep track of the target)
} EmTitleCard;

#endif