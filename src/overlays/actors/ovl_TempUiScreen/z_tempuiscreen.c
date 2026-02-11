/*
 * File: z_tempuiscreen.c
 * Overlay: ovl_TempUiScreen
 * Description: Custom Actor
 */

#include "z_tempuiscreen.h"

#include "play_state.h"


#define FLAGS (0)

void TempUiScreen_Init(Actor* thisx, PlayState* play);
void TempUiScreen_Destroy(Actor* thisx, PlayState* play);
void TempUiScreen_Update(Actor* thisx, PlayState* play);
void TempUiScreen_Draw(Actor* thisx, PlayState* play);

void TempUiScreen_DoNothing(TempUiScreen* this, PlayState* play);


ActorProfile TempUiScreen_Profile = {
    ACTOR_TEMPUISCREEN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(TempUiScreen),
    (ActorFunc)TempUiScreen_Init,
    (ActorFunc)TempUiScreen_Destroy,
    (ActorFunc)TempUiScreen_Update,
    (ActorFunc)TempUiScreen_Draw,
};

void TempUiScreen_Init(Actor* thisx, PlayState* play) {
    TempUiScreen* this = (TempUiScreen*)thisx;

    this->actionFunc = TempUiScreen_DoNothing;
}

void TempUiScreen_Destroy(Actor* thisx, PlayState* play) {
    TempUiScreen* this = (TempUiScreen*)thisx;
}

void TempUiScreen_Update(Actor* thisx, PlayState* play) {
    TempUiScreen* this = (TempUiScreen*)thisx;

    this->actionFunc(this, play);
}

void TempUiScreen_Draw(Actor* thisx, PlayState* play) {
    TempUiScreen* this = (TempUiScreen*)thisx;
}

void TempUiScreen_DoNothing(TempUiScreen* this, PlayState* play) {

}

void IDontFuckinKnowDude(PlayState* play) {
    // IDEK
}