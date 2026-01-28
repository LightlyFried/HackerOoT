beginseg
    name "ovl_Em_Spell_Projectile"
    compress
    include "$(BUILD_DIR)/src/overlays/actors/ovl_em_spell_projectile/z_em_spell_projectile.o"
    include "$(BUILD_DIR)/src/overlays/actors/ovl_em_spell_projectile/ovl_Em_Spell_Projectile_reloc.o"
endseg

beginseg
    name "ovl_Em_Title_Card"
    compress
    include "$(BUILD_DIR)/src/overlays/ui/ovl_em_title_card/z_em_title_card.o"
    include "$(BUILD_DIR)/src/overlays/ui/ovl_em_title_card/ovl_Em_Title_Card_reloc.o"
endseg