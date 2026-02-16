beginseg
    name "ovl_Em_Title_Card"
    compress
    include "$(BUILD_DIR)/src/overlays/ui/ovl_em_title_card/z_em_title_card.o"
    include "$(BUILD_DIR)/src/overlays/ui/ovl_em_title_card/ovl_Em_Title_Card_reloc.o"
endseg

beginseg
    name "ovl_Ui_Sample_Screen"
    compress
    include "$(BUILD_DIR)/src/overlays/ui/ovl_ui_sample_screen/z_ui_sample_screen.o"
    include "$(BUILD_DIR)/src/overlays/ui/ovl_ui_sample_screen/ovl_Ui_Sample_Screen_reloc.o"
endseg