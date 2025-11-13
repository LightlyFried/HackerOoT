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
#include "printf.h"
#include "play_state.h"
#include "widescreen.h"
#include "gfx_setupdl.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h" // Used to draw the debug Bombchu - remove after debugging


#pragma endregion Includes

//=======================[ STATIC DATA ]=====================//
#pragma region Static Data

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

u64 testTex[TEX_LEN(u64, PLACE_NAME_TEX_WIDTH, PLACE_NAME_TEX_HEIGHT, 8)] = {
#include "assets/textures/place_title_cards/gWaterTempleTitleCardENGTex.ia8.inc.c"
};

#pragma endregion Static Data

//==================[ FORWARD DECLARATIONS ]=================//
#pragma region Forward Declarations

void EmTitleCard_Init(Actor* thisx, PlayState* play);
void EmTitleCard_Destroy(Actor* thisx, PlayState* play);
void EmTitleCard_Update(Actor* thisx, PlayState* play);
void EmTitleCard_Draw(Actor* thisx, PlayState* play);

void EmTitleCard_SetupAction(EmTitleCard* this, EmTitleCardActionFunc actionFunc);
void EmTitleCard_ActionIdle(EmTitleCard* this, PlayState* play);

ActorProfile Em_Title_Card_Profile = {
    ACTOR_EM_TITLE_CARD, // TODO: ADD TO ACTOR TABLE `{root}/include/tables/actor_table.h`
    ACTORCAT_PROP, // Define the type of actor
    FLAGS,
    OBJECT_GAMEPLAY_KEEP, // TODO: When finished debugging with placeholder bombchu change to OBJ_EM_TITLE_CARD,
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
    
    this->titleTextureCtx.alpha = 255;
    this->subtitleTextureCtx.alpha = 0;
    this->titleTextureCtx.intensity = 255;
    this->subtitleTextureCtx.intensity = 0;
    //this->titleTextureCtx.alpha = this->subtitleTextureCtx.alpha = 0;

    this->titleTextureCtx.width = PLACE_NAME_TEX_WIDTH;
    this->titleTextureCtx.height = PLACE_NAME_TEX_HEIGHT;

    this->titleTextureCtx.texture = testTex;

    this->titleTextureCtx.x = 50;
    this->titleTextureCtx.y = 50;

    this->subtitleTextureCtx.x = 50;
    this->subtitleTextureCtx.y = 150;

    PRINTF("Title card initialised!");
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

    if(this->titleTextureCtx.alpha > 0) {
        
        s32 doubleWidth = this->titleTextureCtx.width * 2;
        s32 titleX1 = WIDE_INCR((this->titleTextureCtx.x * 4) - (this->titleTextureCtx.width * 2), (WIDE_GET_RATIO * 100.0f));
        s32 titleX2 = titleX1 + (doubleWidth * 2) - 4;
        s32 titleY1 = (this->titleTextureCtx.y * 4) - (this->titleTextureCtx.height * 2);
        s32 titleY2 = titleY1 + (this->titleTextureCtx.height * 4);
        //s32 textureLanguageOffset = this->titleTextureCtx.width * this->titleTextureCtx.height;

        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        // Draw the title
        OVERLAY_DISP = Gfx_SetupDL_52NoCD(OVERLAY_DISP);

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);

        gDPLoadTextureBlock(OVERLAY_DISP++, (u8*)this->titleTextureCtx.texture, G_IM_FMT_IA, G_IM_SIZ_8b,
                            this->titleTextureCtx.width, this->titleTextureCtx.height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                            G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSPTextureRectangle(OVERLAY_DISP++, 0, 0, this->titleTextureCtx.width, this->titleTextureCtx.height - 1, G_TX_RENDERTILE, 0, 0,
                            WIDE_DIV((1 << 10), WIDE_GET_RATIO), 1 << 10);

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}

#pragma endregion Lifecycle Functions

//====================[ ACTION FUNCTIONS ]===================//
#pragma region Action Functions

void EmTitleCard_SetupAction(EmTitleCard* this, EmTitleCardActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EmTitleCard_ActionIdle(EmTitleCard* this, PlayState* play) {
    // Action
}

void EmTitleCard_ActionTitleFadeIn(EmTitleCard* this, PlayState* play) {
    // Action
}

#pragma endregion Action Functions
