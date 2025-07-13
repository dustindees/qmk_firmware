# OLED_ENABLE = yes
# OLED_DRIVER = SSD1306
# OLED_ENABLE = no

RGBLIGHT_ENABLE = yes
RGBLIGHT_LAYERS = yes
RGB_MATRIX_ENABLE = no
# RGB_MATRIX_ENABLE  = yes
# COMBO_ENABLE = yes
# SPLIT_KEYBOARD = yes
CAPS_WORD_ENABLE = yes
# MOUSEKEY_ENABLE = yes
REPEAT_KEY_ENABLE = yes
LTO_ENABLE = yes
LEADER_ENABLE = yes


# Enabling link-time optimization saves about 3 kilobytes.
# https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
# EXTRAFLAGS += -flto
SRC += oneshot.c
SRC += swapper.c
