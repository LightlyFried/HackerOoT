/*
 * File: z_em_spell_projectile.c
 * Overlay: ovl_EmSpellProjectile
 * Description: Custom Actor
 */

#include "z_em_spell_projectile.h"

//========================[ INCLUDES ]=======================//
#pragma region Includes

#include "sfx.h"
#include "z_lib.h"
#include "effect.h"
#include "ichain.h"
#include "sys_math.h"
#include "play_state.h"
#include "sys_math3d.h"
#include "sys_matrix.h"

#include "printf.h"

#include "assets/objects/gameplay_keep/gameplay_keep.h"

#pragma endregion Includes

//==================[ FORWARD DECLARATIONS ]=================//
#pragma region Forward Declarations

void EmSpellProjectile_Init(Actor* thisx, PlayState* play);
void EmSpellProjectile_Destroy(Actor* thisx, PlayState* play);
void EmSpellProjectile_Update(Actor* thisx, PlayState* play);
void EmSpellProjectile_Draw(Actor* thisx, PlayState* play);

void EmSpellProjectile_SetupAction(EmSpellProjectile* this, EmSpellProjectileActionFunc actionFunc);
void EmSpellProjectile_Shoot(EmSpellProjectile* this, PlayState* play);
void EmSpellProjectile_Fly(EmSpellProjectile* this, PlayState* play);
#pragma endregion Forward Declarations

//=======================[ STATIC DATA ]=====================//
#pragma region Static Data

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

ActorProfile Em_Spell_Projectile_Profile = {
    ACTOR_EM_SPELL_PROJECTILE, // TODO: ADD TO ACTOR TABLE `{root}/include/tables/actor_table.h`
    ACTORCAT_ITEMACTION, // Define the type of actor
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EmSpellProjectile),
    (ActorFunc)EmSpellProjectile_Init,
    (ActorFunc)EmSpellProjectile_Destroy,
    (ActorFunc)EmSpellProjectile_Update,
    (ActorFunc)EmSpellProjectile_Draw,
};

static ColliderQuadInit sColliderQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x00000020, HIT_SPECIAL_EFFECT_NONE, 0x01 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_NEAREST | ATELEM_SFX_NONE,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(minVelocityY, -150, ICHAIN_STOP),
};

#pragma endregion Static Data

//==================[ LIFECYCLE FUNCTIONS ]==================//
#pragma region Lifecycle Functions

void EmSpellProjectile_Init(Actor* thisx, PlayState* play) {
    EmSpellProjectile* this = (EmSpellProjectile*)thisx;

    // The spell type and element are u8s packed into the params s16
    // Unpack the type by using a bitwise AND that masks only the lower 8 bits 
    this->spellType = (this->actor.params & 0x00FF);
    // Unpack the element by right shifting the higher 8 bits into place
    this->spellElement = (this->actor.params >> 8);

    Actor_ProcessInitChain(&this->actor, sInitChain);

    // Process specific init stuff for each spell type here
    // switch (this->spellType)
    // {
    //     case EM_SPELL_SLOW_BALL: // Nayru
    //         break;
    //     case EM_SPELL_BOLT: // Farore
    //         break;
    //     case EM_SPELL_LOBBED: // Din
    //         break;
    //     default:
    //         break;
    // }
    this->collider.elem.atDmgInfo.dmgFlags = DMG_ARROW_NORMAL;
    
    Collider_InitQuad(play, &this->collider);
    Collider_SetQuad(play, &this->collider, &this->actor, &sColliderQuadInit);

    EmSpellProjectile_SetupAction(this, EmSpellProjectile_Shoot);
}

void EmSpellProjectile_Destroy(Actor* thisx, PlayState* play) {
    EmSpellProjectile* this = (EmSpellProjectile*)thisx;

    Effect_Delete(play, this->effectIndex);
    Collider_DestroyQuad(play, &this->collider);

    // Reset the player's magic state so they can cast another spell
    // @todo: Implement global cooldown & MP charge
    Magic_Reset(play);
}

void EmSpellProjectile_Update(Actor* thisx, PlayState* play) {
    EmSpellProjectile* this = (EmSpellProjectile*)thisx;
    Player* player = GET_PLAYER(play);
    
    if (!Player_InBlockingCsMode(play, player)) {
        this->actionFunc(this, play);
    }

    //SkelAnime_Update(&this->skelAnime);
}

void EmSpellProjectile_Draw(Actor* thisx, PlayState* play) {
    EmSpellProjectile* this = (EmSpellProjectile*)thisx;
    Gfx_DrawDListOpa(play, gBombchuDL); // Draws a debug bombchu to make sure this works

    // Draw particle trail
}

#pragma endregion Lifecycle Functions

//====================[ ACTION FUNCTIONS ]===================//
#pragma region Action Functions

void EmSpellProjectile_SetupAction(EmSpellProjectile* this, EmSpellProjectileActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EmSpellProjectile_Shoot(EmSpellProjectile* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    switch (this->spellType)
    {
        case EM_SPELL_SLOW_BALL: // Nayru
            Player_PlaySfx(player, NA_SE_IT_EXPLOSION_ICE);
            Actor_SetProjectileSpeed(&this->actor, 10.0f);
            this->timer = 40;
            break;
        case EM_SPELL_BOLT: // Farore
            Player_PlaySfx(player, NA_SE_IT_MAGIC_ARROW_SHOT);
            Actor_SetProjectileSpeed(&this->actor, 60.0f);
            this->timer = 15;
            break;
        case EM_SPELL_LOBBED: // Din
            Player_PlaySfx(player, NA_SE_IT_EXPLOSION_LIGHT);
            this->actor.velocity.y = 14.0f;
            Actor_SetProjectileSpeed(&this->actor, 12.0f);
            this->timer = 10;
            break;
        default:
            break;
    }

    Math_Vec3f_Copy(&this->unk_210, &this->actor.world.pos);
    
    EmSpellProjectile_SetupAction(this, EmSpellProjectile_Fly);
}

void EmSpellProjectile_Fly(EmSpellProjectile* this, PlayState* play) {
    CollisionPoly* hitPoly;
    s32 bgId;
    Vec3f hitPoint;
    Vec3f posCopy;
    s32 atTouched;
    u16 sfxId;
    Actor* hitActor;

    if (DECR(this->timer) == 0) {
        Actor_Kill(&this->actor);
        return;
    }

    // For falloff
    if (this->timer < 7.2f && this->spellType != EM_SPELL_SLOW_BALL) {
        this->actor.gravity = -0.4f;
    }

    atTouched = (this->collider.base.atFlags & AT_HIT);
    if (atTouched || this->touchedPoly) {
        // Spawn a 'bullethole' decal
        EffectSsHitMark_SpawnCustomScale(play, 0, 150, &this->actor.world.pos);


        if(atTouched) { // If we've hit an actor's collider
            PRINTF("---------- We hit an actor!");
            if(hitActor->update != NULL) {
                this->hitActor = hitActor;
                Actor_PlaySfx(&this->actor, NA_SE_IT_ARROW_STICK_CRE);
            }
        } else if (this->touchedPoly) { // If we've hit scene collision
            PRINTF("---------- We hit a surface!");
        } else {
            PRINTF("---------- We hit... something?");
        }

    } else {
        Math_Vec3f_Copy(&this->unk_210, &this->actor.world.pos);
        Actor_MoveXZGravity(&this->actor);
    }

}

#pragma endregion Action Functions
