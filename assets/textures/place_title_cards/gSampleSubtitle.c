#include "ultra64.h"
#include "tex_len.h"
#include "versions.h"
#include "actor.h"


static u64 sSampleSubtitleTex[TEX_LEN(u64, 128, 16, 8)] = {
#include "assets/textures/place_title_cards/gSampleSubtitle.ia8.inc.c"
};