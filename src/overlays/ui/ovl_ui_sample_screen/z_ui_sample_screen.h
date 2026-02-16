#ifndef Z_UI_SAMPLE_SCREEN_H
#define Z_UI_SAMPLE_SCREEN_H

#include "actor.h"

struct UiSampleScreen;

typedef void (*UiSampleScreenActionFunc)(struct UiSampleScreen*, struct PlayState*);

typedef struct UiSampleScreen {
    Actor actor;
    UiSampleScreenActionFunc actionFunc;
} UiSampleScreen;

#endif