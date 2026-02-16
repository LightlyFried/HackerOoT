#ifndef UI_CONTEXT_H
#define UI_CONTEXT_H

#include "z_math.h"

typedef struct UIContext {
    u8 isModalActive; // Whether a full-screen modal is intercepting the game's update/draw calls
    Actor* currentModal;
} UIContext;

#endif