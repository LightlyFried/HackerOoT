/*
 * File: z_em_title_card.c
 * Overlay: ovl_EmTitleCard
 * Description: Custom Actor
 */

#include "z_ui_sample_screen.h"

//========================[ INCLUDES ]=======================//
#pragma region Includes

#include "gfx.h"
#include "actor.h" // For defined PLACE_NAME_TEX_HEIGHT / _WIDTH values
#include "z_lib.h"
#include "printf.h"
#include "play_state.h"
#include "widescreen.h"
#include "gfx_setupdl.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h" // Used to draw the debug Bombchu - remove after debugging

#pragma endregion Includes

//==================[ FORWARD DECLARATIONS ]=================//
#pragma region Forward Declarations

void UiSampleScreen_Init(Actor* thisx, PlayState* play);
void UiSampleScreen_Destroy(Actor* thisx, PlayState* play);
void UiSampleScreen_Update(Actor* thisx, PlayState* play);
void UiSampleScreen_Draw(Actor* thisx, PlayState* play);

void UiSampleScreen_ActionEnter(UiSampleScreen* this, PlayState* play);
void UiSampleScreen_ActionExit(UiSampleScreen* this, PlayState* play);
void UiSampleScreen_ActionIdle(UiSampleScreen* this, PlayState* play);

#pragma endregion Forward Declarations

//=======================[ STATIC DATA ]=====================//
#pragma region Static Data

#define UI_SAMPLE_SCREEN_ACTOR_FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

ActorProfile Ui_Sample_Screen_Profile = {
    ACTOR_UI_SAMPLE_SCREEN,
    ACTORCAT_BG,
    UI_SAMPLE_SCREEN_ACTOR_FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(UiSampleScreen),
    (ActorFunc)UiSampleScreen_Init,
    (ActorFunc)UiSampleScreen_Destroy,
    (ActorFunc)UiSampleScreen_Update,
    (ActorFunc)UiSampleScreen_Draw,
};

static u64 sBorderTex[TEX_LEN(u64, 32, 19, 4)] = {
    #include "assets/textures/icon_item_field_static/gWorldMapAreaBox1Tex.ia4.inc.c"
};

#pragma endregion Static Data

//==================[ LIFECYCLE FUNCTIONS ]==================//
#pragma region Lifecycle Functions

void UiSampleScreen_Init(Actor* thisx, PlayState* play) {
    UiSampleScreen* this = (UiSampleScreen*)thisx;
    this->actionFunc = UiSampleScreen_ActionEnter;
}

void UiSampleScreen_Destroy(Actor* thisx, PlayState* play) {
    UiSampleScreen* this = (UiSampleScreen*)thisx;
    PRINTF("Sample screen destroyed!\n");
}

void UiSampleScreen_Update(Actor* thisx, PlayState* play) {
    UiSampleScreen* this = (UiSampleScreen*)thisx;
    
    if (this->actionFunc != NULL) {
        this->actionFunc(this, play);
    }
}

void UiSampleScreen_Draw(Actor* thisx, PlayState* play) {
    UiSampleScreen* this = (UiSampleScreen*)thisx;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);
    gDPLoadTextureBlock(OVERLAY_DISP++, (u8*)sBorderTex, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 19, 0,
        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(OVERLAY_DISP++, 16, 16, 280, 184 - 1, G_TX_RENDERTILE, 0, 0,
        WIDE_DIV((1 << 10), WIDE_GET_RATIO), 1 << 10);
    
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

#pragma endregion Lifecycle Functions

//====================[ ACTION FUNCTIONS ]===================//
#pragma region Action Functions

void UiSampleScreen_ActionEnter(UiSampleScreen* this, PlayState* play) {
    this->actionFunc = UiSampleScreen_ActionIdle;
}

void UiSampleScreen_ActionExit(UiSampleScreen* this, PlayState* play) {

}

void UiSampleScreen_ActionIdle(UiSampleScreen* this, PlayState* play) {
    
}

#pragma endregion Action Functions
