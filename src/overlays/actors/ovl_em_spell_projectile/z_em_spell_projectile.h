#ifndef Z_EM_SPELL_PROJECTILE_H
#define Z_EM_SPELL_PROJECTILE_H

#include "ultra64.h"
#include "actor.h"
#include "player.h"

struct EmSpellProjectile;

typedef enum EmSpellType {
    EM_SPELL_SLOW_BALL,
    EM_SPELL_BOLT,
    EM_SPELL_LOBBED
} EmSpellType;

typedef enum EmSpellElement {
    EM_ELEMENT_FIRE,
    EM_ELEMENT_ICE,
    EM_ELEMENT_ELECTRIC,
    EM_ELEMENT_LIGHT
} EmSpellElement;

typedef void (*EmSpellProjectileActionFunc)(struct EmSpellProjectile*, struct PlayState*);

typedef struct EmSpellProjectile {
    Actor actor;
    u8 timer; // used for dissapearing when flying or hitting a wall
    Vec3f unk_210; // Copied from EnArrow - maybe rotation?
    u8 touchedPoly;
    Actor* hitActor;
    s32 effectIndex; // A global index for managing/destroying effects
    ColliderQuad collider;
    WeaponInfo weaponInfo;
    EmSpellProjectileActionFunc actionFunc;
    EmSpellType spellType;
    EmSpellElement spellElement;
} EmSpellProjectile;

#endif