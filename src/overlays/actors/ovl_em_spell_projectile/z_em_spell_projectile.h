#ifndef Z_EM_SPELL_PROJECTILE_H
#define Z_EM_SPELL_PROJECTILE_H

#include "ultra64.h"
#include "actor.h"
#include "player.h"

struct EmSpellProjectile;

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
} EmSpellProjectile;

typedef enum EmSpellType {
    EM_SPELL_SLOW_BALL = (1 << 0),
    EM_SPELL_BOLT = (1 << 1),
    EM_SPELL_LOBBED = (1 << 2)
} EmSpellType;

typedef enum EmSpellElement {
    EM_ELEMENT_FIRE = (1 << 0),
    EM_ELEMENT_ICE = (1 << 1),
    EM_ELEMENT_ELECTRIC = (1 << 2),
    EM_ELEMENT_LIGHT = (1 << 3)
} EmSpellElement;

#endif