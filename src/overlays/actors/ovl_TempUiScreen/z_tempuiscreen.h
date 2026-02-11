#ifndef Z_TEMPUISCREEN_H
#define Z_TEMPUISCREEN_H

#include "ultra64.h"
#include "actor.h"
#include "play_state.h"

// Textures for map screen; found in extracted/{build}/
#include "assets/textures/icon_item_field_static/icon_item_field_static.h"

struct TempUiScreen;

typedef void (*TempUiScreenActionFunc)(struct TempUiScreen*, PlayState*);

void IDontFuckinKnowDude(PlayState* play);

typedef struct TempUiScreen {
    Actor actor;
    TempUiScreenActionFunc actionFunc;
} TempUiScreen;

#endif
