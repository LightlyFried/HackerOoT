/*
 * File: z_em_title_card.c
 * Overlay: ovl_EmTitleCard
 * Description: Custom Actor
 */

#include "z_em_title_card.h"

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

//=======================[ STATIC DATA ]=====================//
#pragma region Static Data

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define EM_TITLE_CARD_TITLE_WIDTH PLACE_NAME_TEX_WIDTH // 144
#define EM_TITLE_CARD_TITLE_HEIGHT PLACE_NAME_TEX_HEIGHT // 24
#define EM_TITLE_CARD_SUBTITLE_WIDTH 128
#define EM_TITLE_CARD_SUBTITLE_HEIGHT 16
#define EM_TITLE_CARD_DIVIDER_WIDTH 16
#define EM_TITLE_CARD_DIVIDER_HEIGHT 16

#define EM_TITLE_CARD_SCREEN_POS_X 160 // The centre of the (320x240) screen @todo: account for widescreen (426x240)
#define EM_TITLE_CARD_SCREEN_POS_Y 120 // The centre of the (320x240) screen

#define EM_TITLE_CARD_FADE_IN_A_STEP 8
#define EM_TITLE_CARD_FADE_IN_I_STEP 16
#define EM_TITLE_CARD_DIVIDER_GROW_STEP 6

#define EM_TITLE_CARD_FADE_OUT_A_STEP 5
#define EM_TITLE_CARD_FADE_OUT_I_STEP 10

static u64 sTestTex[TEX_LEN(u64, EM_TITLE_CARD_TITLE_WIDTH, EM_TITLE_CARD_TITLE_HEIGHT, 8)] = {
#include "assets/textures/place_title_cards/gWaterTempleTitleCardENGTex.ia8.inc.c"
};

static u64 sDividerTex[TEX_LEN(u64, 8, 8, 8)] = {
#include "assets/textures/parameter_static/gMagicMeterFillTex.ia8.inc.c"
};

static u64 sSubtitleTex[TEX_LEN(u64, 128, 16, 8)] = {
#include "assets/textures/place_title_cards/gSampleSubtitle.ia8.inc.c"
};

#pragma endregion Static Data

//==================[ FORWARD DECLARATIONS ]=================//
#pragma region Forward Declarations

void EmTitleCard_Init(Actor* thisx, PlayState* play);
void EmTitleCard_Destroy(Actor* thisx, PlayState* play);
void EmTitleCard_Update(Actor* thisx, PlayState* play);
void EmTitleCard_Draw(Actor* thisx, PlayState* play);

void EmTitleCard_SetupAction(EmTitleCard* this, EmTitleCardActionFunc actionFunc);
void EmTitleCard_ActionTitleEnter(EmTitleCard* this, PlayState* play);
void EmTitleCard_ActionIdle(EmTitleCard* this, PlayState* play);
void EmTitleCard_ActionSubtitleReveal(EmTitleCard* this, PlayState* play);
void EmTitleCard_ActionSubtitleMove(EmTitleCard* this, PlayState* play);
void EmTitleCard_ActionAllExit(EmTitleCard* this, PlayState* play);

ActorProfile Em_Title_Card_Profile = {
    ACTOR_EM_TITLE_CARD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EmTitleCard),
    (ActorFunc)EmTitleCard_Init,
    (ActorFunc)EmTitleCard_Destroy,
    (ActorFunc)EmTitleCard_Update,
    (ActorFunc)EmTitleCard_Draw,
};

#pragma endregion Forward Declarations

//==================[ LIFECYCLE FUNCTIONS ]==================//
#pragma region Lifecycle Functions

void EmTitleCard_Init(Actor* thisx, PlayState* play) {
    EmTitleCard* this = (EmTitleCard*)thisx;

    // Somehow determine the correct textures for both the title and subtitle?
    // Also ig set the X&Y coords for each
    
    // The SceneTableEntry struct holds a reference to the title card file in the form of two pointers - 
    // the start and end RAM addresses of the file
    //SceneTableEntry* loadedScene = play->loadedScene;
    //u32 size = loadedScene->titleFile.vromEnd - loadedScene->titleFile.vromStart;
    
    //if ((size > 0) && (size <= 0x1000 * LANGUAGE_MAX)) {
    //    DMA_REQUEST_SYNC(this->titleTextureCtx.texture, loadedScene->titleFile.vromStart, size, __FILE__, __LINE__);
    //}
    
    this->alpha = this->intensity = 0;

    this->titleTextureCtx.width = EM_TITLE_CARD_TITLE_WIDTH;
    this->titleTextureCtx.height = EM_TITLE_CARD_TITLE_HEIGHT;

    this->subtitleTextureCtx.width = EM_TITLE_CARD_SUBTITLE_WIDTH;
    this->subtitleTextureCtx.height = EM_TITLE_CARD_SUBTITLE_HEIGHT;

    this->titleTextureCtx.texture = sTestTex;
    this->dividerTextureCtx.texture = sDividerTex;
    this->subtitleTextureCtx.texture = sSubtitleTex;

    // Set the textures' initial screenpos rects
    s32 doubleWidth = this->titleTextureCtx.width * 2;
    this->titleTextureCtx.x1 = WIDE_INCR((EM_TITLE_CARD_SCREEN_POS_X * 4) - doubleWidth, (WIDE_GET_RATIO * 100.0f));
    this->titleTextureCtx.x2 = this->titleTextureCtx.x1 + (doubleWidth * 2) - 4;
    this->titleTextureCtx.y1 = (EM_TITLE_CARD_SCREEN_POS_Y * 4) - (this->titleTextureCtx.height * 2);
    this->titleTextureCtx.y2 = this->titleTextureCtx.y1 + (this->titleTextureCtx.height * 4);

    this->dividerTextureCtx.x1 = this->titleTextureCtx.x1 + doubleWidth - 16;
    this->dividerTextureCtx.x2 = this->dividerTextureCtx.x1 + 32;
    this->dividerTextureCtx.y1 = this->titleTextureCtx.y2;
    this->dividerTextureCtx.y2 = this->dividerTextureCtx.y1 + EM_TITLE_CARD_DIVIDER_HEIGHT;

    this->subtitleTextureCtx.x1 = this->titleTextureCtx.x1;
    this->subtitleTextureCtx.x2 = this->titleTextureCtx.x2;
    this->subtitleTextureCtx.y1 = this->dividerTextureCtx.y2 + 16;
    this->subtitleTextureCtx.y2 = this->subtitleTextureCtx.y1;

    this->subtitleCurrentHeight = 0;

    this->keyframeCountdown = 60;
    this->actionFunc = EmTitleCard_ActionTitleEnter;
}

void EmTitleCard_Destroy(Actor* thisx, PlayState* play) {
    EmTitleCard* this = (EmTitleCard*)thisx;
}

void EmTitleCard_Update(Actor* thisx, PlayState* play) {
    EmTitleCard* this = (EmTitleCard*)thisx;
    
    if (this->actionFunc != NULL) {
        this->actionFunc(this, play);
    }
}

void EmTitleCard_Draw(Actor* thisx, PlayState* play) {
    EmTitleCard* this = (EmTitleCard*)thisx;

    // Early return if the title's alpha is 0 - either before the animation starts or after it ends
    if(this->alpha == 0) {
        return;
    }
        
    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    // Draw the title texture
    OVERLAY_DISP = Gfx_SetupDL_52NoCD(OVERLAY_DISP);

    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, this->intensity, this->intensity, this->intensity, this->alpha);

    gDPLoadTextureBlock(OVERLAY_DISP++, (u8*)this->titleTextureCtx.texture, G_IM_FMT_IA, G_IM_SIZ_8b,
            this->titleTextureCtx.width, this->titleTextureCtx.height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
            G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(OVERLAY_DISP++, this->titleTextureCtx.x1, this->titleTextureCtx.y1, this->titleTextureCtx.x2, this->titleTextureCtx.y2 - 1, G_TX_RENDERTILE, 0, 0,
            WIDE_DIV((1 << 10), WIDE_GET_RATIO), 1 << 10);

    // Draw the divider texture, calculating the current width and position
    gDPLoadMultiBlock_4b(OVERLAY_DISP++, sDividerTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, EM_TITLE_CARD_DIVIDER_WIDTH, EM_TITLE_CARD_DIVIDER_HEIGHT, 0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(OVERLAY_DISP++, this->dividerTextureCtx.x1, this->dividerTextureCtx.y1, this->dividerTextureCtx.x2, this->dividerTextureCtx.y2, G_TX_RENDERTILE, 0, 0,
            WIDE_DIV((1 << 10), WIDE_GET_RATIO), 1 << 10);
    

    // Draw the subtitle
    if(this->subtitleCurrentHeight > 0) {

        gDPLoadTextureBlock(OVERLAY_DISP++, (u8*)this->subtitleTextureCtx.texture, G_IM_FMT_IA, G_IM_SIZ_8b,
                this->subtitleTextureCtx.width, this->subtitleTextureCtx.height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        // By default, texture rects will always draw from the top-left corner of the texture. Since the subtitle is sliding *from the bottom*, it
        // needs to draw the bottom 'slice' of the texture. The s and t coordinates (texture space's versions of x and y respectively) are set in
        // gSPTextureRectangle below.
        // In OOT, texture coordinates use the s10.5 fixed-point format, meaning it's a signed format with 10 integer bits and 5 fractional bits,
        // if you want to encode 1.0 in s10.5 you'd do 1 * 32 or 1 << 5, or encoding 0.5 it's 0.5 * 32 or some equivalent
        // Say for a texture with dimensions 32x32 and you want to draw the lower 32x16 slice, you'd set S=(0 * 32) and T=(16 * 32) to set the
        // (s,t) coordinates that will map onto (ulx, uly)
        s16 texT = (EM_TITLE_CARD_SUBTITLE_HEIGHT - this->subtitleCurrentHeight) * 32;

        // Find the bottom of the rect - don't forget texture space is 4x pixel space
        this->subtitleTextureCtx.y2 = this->subtitleTextureCtx.y1 + (this->subtitleCurrentHeight * 4);

        gSPTextureRectangle(OVERLAY_DISP++, this->subtitleTextureCtx.x1, this->subtitleTextureCtx.y1, this->subtitleTextureCtx.x2, this->subtitleTextureCtx.y2 - 1, G_TX_RENDERTILE, 0, texT,
                WIDE_DIV((1 << 10), WIDE_GET_RATIO), 1 << 10);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

#pragma endregion Lifecycle Functions

//====================[ ACTION FUNCTIONS ]===================//
#pragma region Action Functions

void EmTitleCard_SetupAction(EmTitleCard* this, EmTitleCardActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EmTitleCard_ActionIdle(EmTitleCard* this, PlayState* play) {
    if(DECR(this->keyframeCountdown)) {

    } else {
        // No need to restart the timer as the overlay will destroy itself when alpha hits 0
        this->actionFunc = EmTitleCard_ActionAllExit;
    }
}

void EmTitleCard_ActionTitleEnter(EmTitleCard* this, PlayState* play) {
    if(DECR(this->keyframeCountdown)) {
            Math_StepToS(&this->alpha, 255, EM_TITLE_CARD_FADE_IN_A_STEP);
            Math_StepToS(&this->intensity, 255, EM_TITLE_CARD_FADE_IN_I_STEP);

            // Step the divider's x values to the title's x values to effectively 'stretch' the divider during the animation
            Math_StepToS(&this->dividerTextureCtx.x1, this->titleTextureCtx.x1, EM_TITLE_CARD_DIVIDER_GROW_STEP);
            Math_StepToS(&this->dividerTextureCtx.x2, this->titleTextureCtx.x2, EM_TITLE_CARD_DIVIDER_GROW_STEP);
    } else {
        // Once complete, make sure the vars are at their max values instead of
        // the second-to-last step's values
        this->alpha = 255;
        this->intensity = 255;

        this->dividerTextureCtx.x1 = this->titleTextureCtx.x1;
        this->dividerTextureCtx.x2 = this->titleTextureCtx.x2;

        this->keyframeCountdown = 32;
        this->actionFunc = EmTitleCard_ActionSubtitleReveal;
    }
}

void EmTitleCard_ActionSubtitleReveal(EmTitleCard* this, PlayState* play) {
    if(DECR(this->keyframeCountdown)) {
        if(this->keyframeCountdown % 2 == 0) {
            Math_StepToS(&this->subtitleCurrentHeight, EM_TITLE_CARD_SUBTITLE_HEIGHT, 1);
        }
    } else {
        this->subtitleCurrentHeight = EM_TITLE_CARD_SUBTITLE_HEIGHT;

        this->keyframeCountdown = 32;
        this->actionFunc = EmTitleCard_ActionSubtitleMove;
    }
}

void EmTitleCard_ActionSubtitleMove(EmTitleCard* this, PlayState* play) {
    if(DECR(this->keyframeCountdown)) {
        if(this->keyframeCountdown % 2 == 0) {
            this->subtitleTextureCtx.y1 += 4;
            this->subtitleTextureCtx.y2 += 4;
        }
    } else {
        //this->subtitleCurrentHeight = EM_TITLE_CARD_SUBTITLE_HEIGHT;

        this->keyframeCountdown = 40;
        this->actionFunc = EmTitleCard_ActionIdle;
    }
}

void EmTitleCard_ActionAllExit(EmTitleCard* this, PlayState* play) {
    if(this->alpha > 0) {
        Math_StepToS(&this->alpha, 0, EM_TITLE_CARD_FADE_OUT_A_STEP);
        Math_StepToS(&this->intensity, 0, EM_TITLE_CARD_FADE_OUT_I_STEP);
    } else {
        Actor_Kill(&this->actor);
    }
}

#pragma endregion Action Functions
