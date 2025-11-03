#ifndef EM_SAVE_H
#define EM_SAVE_H

#pragma region Static Data

typedef enum EmEventFlags {
    EM_EVENT_MET_IMPA_IN_TAVERN = 0x0001,
    EM_EVENT_MET_IMPA_AT_NIGHT,
    EM_EVENT_MET_VASU,
    EM_EVENT_MET_TINGLE,
    EM_EVENT_TINGLE_BOUGHT_PHERO_MAP,
    EM_EVENT_TINGLE_BOUGHT_ELDIN_MAP
} EmEventFlags;

typedef enum EmMagicRingIndices {
    EM_RING_FRIENDSHIP = 0,       // Heart drops +
    EM_RING_DMG_LV1,              // DMG+  | DEF-
    EM_RING_DMG_LV2,              // DMG++ | DEF-
    EM_RING_DMG_LV3,              // DMG+++| DEF-
    EM_RING_DEF_LV1,              // DEF+  | DMG-
    EM_RING_DEF_LV2,              // DEF++ | DMG-
    EM_RING_DEF_LV3,              // DEF+++| DMG-
    EM_RING_LOOT_MAGNET,          // Drops within range float to player
    EM_RING_RUPEE_DROPS,          // Rupee drops + (or rupee *size* + ?)
    EM_RING_MAX
} EmMagicRingIndices;

typedef enum EmMagicRingStatuses {
    EM_RING_STATUS_UNOBTAINED = 0,
    EM_RING_STATUS_OBTAINED,
    EM_RING_STATUS_APPRAISED,
    EM_RING_STATUS_EQUIPPED
} EmMagicRingStatuses;

typedef struct EmSaveInfo
{
    u16 emEventTable[8];
    u8 emMagicRings[EM_RING_MAX];
} EmSaveInfo;

#pragma endregion Static Data


#pragma region Globals

#define EM_GLOBAL_PLAYER_CHILD_SPEED_MULT 1.1f
#define EM_GLOBAL_PLAYER_ADULT_SPEED_MULT 1.2f
#define EM_GLOBAL_CURRENT_SPEED_MULT ((gSaveContext.save.linkAge == LINK_AGE_ADULT) ? EM_GLOBAL_PLAYER_ADULT_SPEED_MULT : EM_GLOBAL_PLAYER_CHILD_SPEED_MULT)

#pragma endregion Globals


#pragma region Event Flags

// Shifts the given flag RIGHT 4 bits (essentially dividing the number by 16)
// This gets the index of the flag's u16 (or 16-bit 'block') in `emEventTable[]`
// This and `EM_EVT_FLAG_MASK` have their name changed from `EM_EVENT_FLAG_MASK` format
// just so they don't show up in intellisense when I'm searching through the enum
#define EM_EVT_BLOCK_INDEX(flag) ((flag) >> 4)

// Masks the given flag in a 16 bit block
// (flag) & 0xF) is equivalent to `16 % flag` - i.e the remainder when dividing the flag by 16.
// This gives a number between 0 and 15, and bitshifting 1 by that many digits results in a single
// bit in a 16-bit number
#define EM_EVT_FLAG_MASK(flag) (1 << ((flag) & 0xF))

// Event flags getters & setters
#define EM_GET_EVENT_FLAG(flag) (gSaveContext.save.emSaveInfo.emEventTable[EM_EVT_BLOCK_INDEX(flag)] & EM_EVT_FLAG_MASK(flag))     // Get the value of an event flag. Tracked in `em_save.h`, all enum entries should start with `EM_EVENT_`
#define EM_SET_EVENT_FLAG(flag) (gSaveContext.save.emSaveInfo.emEventTable[EM_EVT_BLOCK_INDEX(flag)] |= EM_EVT_FLAG_MASK(flag))    // ENABLE a given event flag. Tracked in `em_save.h`, all enum entries should start with `EM_EVENT_`
#define EM_CLEAR_EVENT_FLAG(flag) (gSaveContext.save.emSaveInfo.emEventTable[EM_EVT_BLOCK_INDEX(flag)] &= ~EM_EVT_FLAG_MASK(flag)) // DISABLE a given event flag. Tracked in `em_save.h`, all enum entries should start with `EM_EVENT_`

#pragma endregion Event Flags


#pragma region Magic Rings

#define EM_GET_RING_STATUS(ringIndex) (gSaveContext.save.emSaveInfo.emMagicRings[ringIndex])
#define EM_SET_RING_STATUS(ringIndex, statusFlag) (gSaveContext.save.emSaveInfo.emMagicRings[ringIndex] |= statusFlag)

#pragma endregion Magic Rings


#endif