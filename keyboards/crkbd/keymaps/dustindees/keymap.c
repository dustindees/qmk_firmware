/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "oneshot.h"
#include "swapper.h"
#include <stdio.h>
#include "action_layer.h"
#include "timer.h"
#define LEDS_PER_HALF 24

#ifdef LEADER_ENABLE
    #include "process_leader.h"
    extern bool leading;
#endif

static uint32_t last_keypress_timer = 0;
static bool sym2_active = false;

enum custom_keycodes {
    right_ret = SAFE_RANGE,
    line_begin_del,
    line_end_del,
    dup_tab,
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    SW_WIN,
    MAGIC_LT,
    MAGIC_CT,
    MAGIC_RI,
    //MAGIC_RT,
    ADDR_BAR,
    PREV_TAB,
    NEXT_TAB,
    CTRL_ALT_DEL,
    //MAGIC_LI,
    //MAGIC_RI,
};


enum layers {
    _DEF,       //default alpha layer
    _NAV1,      //navigation
    _NAV2,      //navigation
    _SYM1,      //symbols
    _SYM2,      //symbols
    _NUM,       //numbers
    _FUN,       //function/media keys
    _DEL,       //homerow del keys
    //_MOU,       //mouse keys
};

#ifdef OLED_ENABLE
#include "layer_state_reader.c"

// static void render_logo(void) {
//   static const char PROGMEM logo[] = {
//       0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
//       0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
//       0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
//       0};

//   oled_write_P(logo, false);
// }

// static void render_logo(void) {
//     static const char PROGMEM raw_logo[] = {
//         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,192,240,248,252,248,248,248,248,248,248,248,248,248,248,248,248,248,216,216,216,216,232,236,236,228,228,224,240,240,240,240,240,240,248,248,248,248,248,248,252,252,248,224,192,192,192,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16,156,188,188,188,188,156,220,220,252,252,222,222,254,254,255,223,223,255,255,255,255,223,223,223,255,239,239,255,255,255,255,255,255,255,255, 63,127,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,253,253,253,253,253,253, 12, 12,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1, 15,  7,  7,  7,  3,129,129,193,193,225,225,225, 97, 99, 99,115,115,115, 51, 59, 59, 63,191,191,159,159,159,143,199,193,193,224,224,240,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,254,255,255,253,253,253,253,253,249,249,249,251,251,123,115,115,115, 51, 51, 35,  7,  7,  7,  7,  7,  7,  7,  6,  4,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  9,  9, 13, 12, 12, 12, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 11,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     };
//     oled_write_raw_P(raw_logo, sizeof(raw_logo));
// }

static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        0,  0,  0,  0,248,248, 64, 64,248,248,192,224,224,224,224,192,  0,  8,248,248,  0,  0,  0,  0,248,248,  0,  0,192,224, 96, 32,224,224,128,  0,  0,192,192,128,  0,  0,  0,  0,  0,  0,  0,224,224,192,192,224,224,224,224, 48, 48,224,192,224,224, 96, 48, 96, 96,  0,  8,248,248,  0,  0,224,224, 48,224,248,248,  0,  0,120,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  3,  3,  1,  0,  0,  1,  3,  1,  1,  0,  0,  0,  3,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  3,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  1,129,  1,129,  0,128,129,  1,  1,  1,  1,  0,128,129,129,128,128,128,128,128,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,  0,  0,128,128,128,128,128,128,128,128,128,128,128,  0,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,
        0,  0,  0,  0, 28,252,254,195,195, 99,115, 59, 59,254,254,  0,  0, 24, 24,112, 96,192,192,224,112, 56, 24, 24,  0,255,255,255,  3,  3,  3,  3,  3,255,254,252,  0,255,255,255,115,115,115,115,115, 99,  3,  3,  0,248,248,254,206,199,195,199,206,254,248,248,  0,255,255,255,  3,  3,  3,  3,  3,255,254,254,  0, 31,255,255,115,115,115,115,115,115,222,222,128,  0,255,255,243, 51,115, 51,115, 51, 35,  3,  1,  0,255,255,255, 51, 51, 51, 51, 51, 51,  3,  1,  0,255,255,255, 51, 51, 51, 51, 51, 51,  3,  1,  0,  0,  0,
        0,  0,  0,  0,  0,  3,  3, 15, 14, 14, 14, 14, 14,  7,  3,  1,  0, 14, 14,  7,  3,  1,  0,  3,  3, 15, 14,  4,  0, 15, 15, 15, 14, 14, 14, 14, 14,  7,  3,  3,  0,  7, 15,  7,  6,  6,  6,  6,  6,  6,  6,  6,  0,  7,  7,  7,  0,  0,  0,  0,  0,  7,  7,  7,  0,  7,  7,  7,  6,  6,  6,  6,  6,  7,  3,  3,  0,  0,  7,  7,  6,  6,  6,  6,  6,  6,  3,  3,  1,  0,  7,  7,  7,  6,  6,  6,  6,  6,  6,  6,  6,  0,  7,  7,  7,  6,  6,  6,  6,  6,  6,  6,  6,  0,  7,  7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

bool oled_task_user(void) {

    if (is_keyboard_master()) {
        // Clear the OLED
        oled_clear();

        // Show the current layer
        oled_write_ln(read_layer_state(), false);

        // You can add more info below, like modifier state, caps word, etc.
        // Example:
        // oled_write_ln(is_caps_word_on() ? "Caps Word: ON" : "Caps Word: OFF", false);
    }
    else {
        render_logo();
    }

    return false;
}
#endif


const rgblight_segment_t PROGMEM layer_def_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_WHITE},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_WHITE}
);
const rgblight_segment_t PROGMEM layer_nav1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_AZURE},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_AZURE}
);
const rgblight_segment_t PROGMEM layer_nav2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_CYAN},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_CYAN}
    // {0, LEDS_PER_HALF, HSV_BLUE},
    // {LEDS_PER_HALF, LEDS_PER_HALF, HSV_BLUE}
);
const rgblight_segment_t PROGMEM layer_sym1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_CYAN},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_CYAN}
);
const rgblight_segment_t PROGMEM layer_sym2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_GREEN},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_GREEN}
);
const rgblight_segment_t PROGMEM layer_num_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_SPRINGGREEN},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_SPRINGGREEN}
);
const rgblight_segment_t PROGMEM layer_fun_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_YELLOW},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_YELLOW}
);
const rgblight_segment_t PROGMEM layer_del_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_RED},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_RED}
);
const rgblight_segment_t PROGMEM layer_mou_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, LEDS_PER_HALF, HSV_CORAL},
    {LEDS_PER_HALF, LEDS_PER_HALF, HSV_CORAL}
);
const rgblight_segment_t PROGMEM modkeys_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {6, 5, HSV_WHITE}, // For example: highlight LED 4 (adjust to your key index)
    {30, 5, HSV_WHITE} // Add more LEDs as needed
);
const rgblight_segment_t PROGMEM leader_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {6, 5, HSV_BLACK},
    {30, 5, HSV_BLACK}
);


// Combine into array of layers
const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer_nav1_layer,
    layer_nav2_layer,
    layer_sym1_layer,
    layer_sym2_layer,
    layer_num_layer,
    layer_fun_layer,
    layer_del_layer,
    //layer_mou_layer,
    modkeys_layer,
    leader_layer
);

bool sw_win_active = false;

void matrix_scan_user(void) {
    static bool mod_layer_on = false;

    bool mods_active = get_mods() || get_oneshot_mods();

    // Modifier lighting
    if (mods_active && !mod_layer_on) {
        rgblight_set_layer_state(7, true);
        mod_layer_on = true;
    } else if (!mods_active && mod_layer_on) {
        rgblight_set_layer_state(7, false);
        mod_layer_on = false;
    }
    #ifdef LEADER_ENABLE
        // Leader key lighting
        static bool leader_layer_on = false;
        bool leader_active = leading;

        if (leader_active && !leader_layer_on) {
            rgblight_set_layer_state(8, true);
            leader_layer_on = true;
        } else if (!leader_active && leader_layer_on) {
            rgblight_set_layer_state(8, false);
            leader_layer_on = false;
        }
    #endif
}

// TO(_NAV1) fix this before compiling

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEF] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     KC_B, KC_F, LT(_NAV1, KC_L), LT(_NAV2, KC_K), LT(_FUN, KC_Q),              KC_P, LT(_NAV2, KC_W), LT(_NAV1, KC_O), KC_U, KC_COMM,
  //|--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------|
     LSFT_T(KC_N), LALT_T(KC_S), LGUI_T(KC_H), LCTL_T(KC_T), KC_M,              KC_Y, LCTL_T(KC_C), LGUI_T(KC_A), LALT_T(KC_E), LSFT_T(KC_I),
  //|--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------|
     LT(_FUN, KC_X), KC_V, KC_J, LT(_NUM, KC_D), KC_Z,                          MAGIC_RI, LT(_NUM, KC_G), KC_QUOTE, KC_SLSH, LT(_FUN, KC_DOT),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                             MAGIC_LT, LT(_SYM1, KC_R), MAGIC_CT,     OS_SHFT, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  ),


//       [_DEF] = LAYOUT_split_3x5_3(
//   //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_Q, KC_L, LT(_NAV1, KC_M), LT(_NAV2, MAGIC_LI), KC_B,                               KC_P, LT(_NAV2, KC_Y), LT(_NAV1, KC_O), KC_J, KC_COMM,
//   //|--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------|
//      LSFT_T(KC_N), LALT_T(KC_R), LGUI_T(KC_T), LCTL_T(KC_H), KC_F,              MAGIC_RI, RCTL_T(KC_C), RGUI_T(KC_A), RALT_T(KC_I),   KC_U,
//   //|--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------|
//      LT(_FUN, KC_Z), LT(_MOU, KC_X), KC_V, LT(_NUM, KC_D), KC_K,                KC_G, LT(_NUM, KC_W), KC_QUOTE, KC_SLSH, LT(_FUN, KC_DOT),
//   //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                              TO(_NAV1), LT(_SYM1, KC_E), LT(_SYM2, KC_S),     OS_SHFT, LT(_DEL, KC_SPC), QK_LEAD
//                                       //`--------------------------'  `--------------------------'

//   ),

//OS_SHFT, LALT_T(A(KC_D)), LGUI_T(RCS(KC_TAB)), LCTL_T(C(KC_TAB)), KC_TAB,
    // NAV1 WINDOWS
    [_NAV1] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    dup_tab, C(KC_5),  C(KC_W), SW_WIN, C(S(KC_T)),                             KC_PAGE_UP,    KC_HOME,    KC_UP,    RCS(KC_LEFT),    S(KC_HOME),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT, LALT_T(ADDR_BAR), LGUI_T(PREV_TAB), LCTL_T(NEXT_TAB), KC_TAB,      XXXXXXX, KC_LEFT,   KC_DOWN, KC_RIGHT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    C(G((KC_LEFT))), XXXXXXX, XXXXXXX, MO(_NUM), C(G((KC_RIGHT))),              KC_PAGE_DOWN, LT(_NUM, KC_END), XXXXXXX, RCS(KC_RIGHT), S(KC_END),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF), MO(_SYM1),  MAGIC_CT,     OS_SHFT, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  ),

    // NAV2 LINUX
    [_NAV2] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    XXXXXXX, dup_tab, G(KC_LEFT), G(KC_RIGHT), XXXXXXX,                        XXXXXXX, XXXXXXX, KC_UP, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT, LALT_T(G(KC_3)), LGUI_T(G(KC_2)), LCTL_T(G(KC_1)), KC_TAB,        XXXXXXX,  KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    XXXXXXX, LSG(KC_3), LSG(KC_2), LSG(KC_1), XXXXXXX,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF),   MO(_SYM1),  MAGIC_CT,     OS_SHFT, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  ),

    // SYM1
    [_SYM1] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    XXXXXXX, C(KC_A), C(KC_C), C(KC_V), C(KC_X),                               XXXXXXX, KC_TILD, KC_DOT, KC_SLSH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    XXXXXXX, KC_SEMICOLON, S(KC_SEMICOLON), KC_ESC, KC_MINUS,                   S(KC_SLSH), KC_GRAVE, KC_QUOT, S(KC_QUOT), XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    CTRL_ALT_DEL, XXXXXXX, XXXXXXX, TO(_NUM), XXXXXXX,                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF),   XXXXXXX,  MAGIC_CT,     CW_TOGG, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  ),

    // SYM2
    [_SYM2] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    S(KC_8), S(KC_BSLS), S(KC_COMMA),  S(KC_DOT), S(KC_MINUS),                 S(KC_GRV), S(KC_LBRC), S(KC_RBRC), S(KC_4), S(KC_6),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_BSLS, KC_EQL, KC_PLUS, S(KC_1), KC_MINUS,                               S(KC_SLSH),  S(KC_9), S(KC_0), S(KC_3), S(KC_2),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_SLSH, XXXXXXX, XXXXXXX, MO(_NUM), S(KC_4),                                S(KC_5), KC_LBRC, KC_RBRC, KC_SLSH, S(KC_7),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF),   MO(_SYM1),  MAGIC_CT,     KC_CAPS, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  ),


    // DEL
    [_DEL] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     XXXXXXX, line_end_del, C(KC_DEL), KC_DEL, XXXXXXX,                         C(KC_A), C(KC_C), C(KC_V), C(KC_X), XXXXXXX,
  //|--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------|
     XXXXXXX, line_begin_del, C(KC_BSPC), KC_BSPC, XXXXXXX,                     XXXXXXX, KC_ENT, right_ret, S(KC_ENT), XXXXXXX,
  //|--------+--------+--------+--------+--------|                              |--------+--------+--------+--------+--------|
     XXXXXXX, XXXXXXX, XXXXXXX, TO(_NUM), XXXXXXX,                                XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF),   MO(_SYM1),  MAGIC_CT,         OS_SHFT, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'

  ),

    [_FUN] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_PRINT_SCREEN, RM_ON, LM_ON, LM_OFF, LM_TOGG,                               XXXXXXX, KC_F7, KC_F8, KC_F9, KC_F10,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_SCROLL_LOCK, RM_OFF, LM_NEXT, LM_PREV, XXXXXXX,                               XXXXXXX,  KC_F4, KC_F5, KC_F6, KC_F11,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_INSERT, RM_NEXT, LM_BRIU, MO(_NUM), XXXXXXX,                               XXXXXXX, KC_F1, KC_F2, KC_F3, KC_F12,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF),   MO(_SYM1),  MAGIC_CT,     OS_SHFT, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  ),

//     // MOU - for mouse keys
//     [_MOU] = LAYOUT_split_3x5_3(
//   //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX, MS_BTN1, MS_BTN2, MS_BTN3, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX,  MS_LEFT, MS_DOWN, MS_UP, MS_RGHT,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX, MS_WHLU, MS_WHLD, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                           TO(_DEF),   XXXXXXX,  XXXXXXX,     OS_SHFT, LT(_DEL, KC_SPC), TO(_DEF)
//                                       //`--------------------------'  `--------------------------'
//   ),

    // NUM
    [_NUM] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_BACKSLASH, KC_ASTERISK, KC_SLSH, KC_DLR, KC_TILD,                       KC_PLUS, KC_7, KC_8, KC_9, KC_X,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    OS_SHFT, OS_ALT, OS_GUI, OS_CTRL, S(KC_8),                                 KC_MINS, KC_4, KC_5, KC_6, KC_DOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_NUM_LOCK, KC_NO, KC_NUM_LOCK, KC_NO, KC_PERC,                           KC_EQL, KC_1, KC_2, KC_3, KC_0,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(_DEF), MO(_SYM1),  MAGIC_CT,     KC_DOT, LT(_DEL, KC_SPC), QK_LEAD
                                      //`--------------------------'  `--------------------------'
  )

//     [_SYM1] = LAYOUT_split_3x5_3(
//   //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//   //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                           XXXXXXX,   XXXXXXX,  XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
//                                       //`--------------------------'  `--------------------------'
//   ),

};


bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false; // Disable Flow Tap on hotkeys.
    }
    // Remove keys to disable flow tap for them
    switch (get_tap_keycode(keycode)) {
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
        case KC_SCLN:
        case KC_SLSH:
            return true;
    }
    return false;
}


// I'll need to update both of these as I fill in my layer keys
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case KC_LSFT:
        case KC_RALT:
            return true;
        default:
            return false;
    }
}

// I'll need to update both of these as I fill in my layer keys
bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case KC_RALT:
        case KC_LSFT:
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_GUI:
            return true;
        default:
            return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;
bool os_shft_sent = false;
bool os_ctrl_sent = false;
bool os_alt_sent  = false;
bool os_gui_sent  = false;



void leader_end_user(void) {
    if (leader_sequence_one_key(KC_F)) {
        SEND_STRING("Test.");
    } else if (leader_sequence_two_keys(KC_GRAVE, KC_U)) {
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_4);
        unregister_code(KC_LALT);
    } else if (leader_sequence_one_key(KC_UP)) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_4);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_one_key(KC_DOWN)) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_5);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_one_key(KC_RIGHT)) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_6);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_one_key(KC_LEFT)) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_7);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_three_keys(LT(_NUM, KC_D), LALT_T(KC_E), LT(_NAV1, KC_L))) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_3);
        tap_code16(KC_KP_5);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_three_keys(LCTL_T(KC_T), LGUI_T(KC_H), LALT_T(KC_E))) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_3);
        tap_code16(KC_KP_3);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_three_keys(LCTL_T(KC_T), LGUI_T(KC_A), KC_U)) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_3);
        tap_code16(KC_KP_1);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_three_keys(KC_P, LGUI_T(KC_H), LSFT_T(KC_I))) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_3);
        tap_code16(KC_KP_2);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_three_keys(KC_P, LSFT_T(KC_I), LALT_T(KC_E))) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_7);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    } else if (leader_sequence_three_keys(LSFT_T(KC_I), LSFT_T(KC_N), KC_F)) {
        tap_code16(KC_NUM_LOCK);
        register_code(KC_LALT);
        tap_code16(KC_KP_2);
        tap_code16(KC_KP_3);
        tap_code16(KC_KP_6);
        unregister_code(KC_LALT);
        tap_code16(KC_NUM_LOCK);
    }
}


bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,

                            uint8_t* remembered_mods) {
    switch (keycode) {
        case CW_TOGG:
        case KC_ESC:
        case KC_BSPC:
        case KC_DEL:

        case MAGIC_LT:
        case MAGIC_CT:
        case MAGIC_RI:
            return false;  // Magic keys will ignore the above keycodes.
    }
    return true;  // Other keys can be repeated.

}

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`.
#define MAGIC_STRING(str, repeat_keycode) \
        magic_send_string_P(PSTR(str), (repeat_keycode))

static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
    uint8_t saved_mods = 0;

  if (is_caps_word_on()) { // If Caps Word is on, save the mods and hold Shift.
    saved_mods = get_mods();
    register_mods(MOD_BIT(KC_LSFT));
  }

  send_string_with_delay_P(str, TAP_CODE_DELAY);  // Send the string.
  set_last_keycode(repeat_keycode); // 2024-03-09 Disabled sending of string for mag-rep / rep-mag consistency.

  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}


static void process_center_thumb_magic(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case LT(_NAV1, KC_O): { MAGIC_STRING("g",         KC_NO); } break;
        case LT(_NUM, KC_G): { MAGIC_STRING("o",         KC_NO); } break;
        case LT(_NAV1, KC_L): { MAGIC_STRING("ove",         KC_SPC); } break;
        case LCTL_T(KC_T): { MAGIC_STRING("ion",         KC_SPC); } break;
        case LALT_T(KC_S): { MAGIC_STRING("ion",         KC_SPC); } break;
        case LGUI_T(KC_A): { MAGIC_STRING("u",         KC_NO); } break;
        case KC_U: { MAGIC_STRING("a",         KC_NO); } break;
        case KC_P: { MAGIC_STRING("a",         KC_NO); } break;
    }
}
// static void process_right_thumb_magic(uint16_t keycode, uint8_t mods) {
//     switch (keycode) {
//         case LT(_NAV1, KC_O): { MAGIC_STRING("g",         KC_NO); } break;
//         case LT(_NUM, KC_G): { MAGIC_STRING("o",         KC_NO); } break;
//         case LT(_NAV1, KC_L): { MAGIC_STRING("ove",         KC_SPC); } break;
//         case LCTL_T(KC_T): { MAGIC_STRING("ion",         KC_SPC); } break;
//     }
// }

static void process_left_thumb_magic(uint16_t keycode, uint8_t mods) {
    if (timer_elapsed32(last_keypress_timer) < 500) {
        switch (keycode) {
            case KC_B: { MAGIC_STRING("ecause",         KC_SPC); } break;
            case KC_P: { MAGIC_STRING("eople",         KC_SPC); } break;
            case KC_COMM: { MAGIC_STRING(" but ",         KC_SPC); } break;
            case LT(_NUM, KC_G): { MAGIC_STRING("o",         KC_NO); } break;
            case LT(_NAV1, KC_L): { MAGIC_STRING("ive",         KC_SPC); } break;
            case KC_M: { MAGIC_STRING("ent",         KC_SPC); } break;
            case LCTL_T(KC_T): { MAGIC_STRING("ment",         KC_SPC); } break;
        }
    } else {
        layer_move(_NAV1);
    }
}

// Could use timer trick here too if ever necessary
static void process_right_index_magic(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_F: { MAGIC_STRING("u",         KC_NO); } break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {
        case MAGIC_LT: { process_left_thumb_magic(get_last_keycode(), get_last_mods()); set_last_keycode(KC_SPC); } return false;
        case MAGIC_RI: { process_right_index_magic(get_last_keycode(), get_last_mods()); set_last_keycode(KC_SPC); } return false;
        //case LT(_NAV2, MAGIC_LI): { process_left_index_magic(get_last_keycode(), get_last_mods()); set_last_keycode(KC_SPC); } return false;
        // case LT(_NAV1, MAGIC_LT): {
        //     if (record->tap.count && record->event.pressed) {
        //         process_left_thumb_magic(get_last_keycode(), get_last_mods()); set_last_keycode(KC_SPC);
        //     } else if (!record->tap.count && record->event.pressed) {
        //         layer_move(_NAV1);
        //     }
        //     return false;
        // }

        // Added sym2_active to better mimic momentary layer behavior
        case MAGIC_CT: {
            if (record->event.pressed) {
                if (timer_elapsed32(last_keypress_timer) < 300) {
                    process_center_thumb_magic(get_last_keycode(), get_last_mods());
                    set_last_keycode(KC_SPC);
                    sym2_active = false;
                } else {
                    layer_on(_SYM2);
                    sym2_active = true;
                }
            } else {
                if (sym2_active) {
                    layer_off(_SYM2);
                    sym2_active = false;
                }

            }
            return false;
        }
        // case MAGIC_RT: {
        //     if (record->event.pressed) {
        //         if (timer_elapsed32(last_keypress_timer) < 300) {
        //             process_right_thumb_magic(get_last_keycode(), get_last_mods());
        //             set_last_keycode(KC_SPC);
        //         } else {
        //             tap_code(KC_L);
        //             tap_code16(QK_LEAD);
        //         }
        //     return false;
        //     }
        // }
        // case MAGIC_RT: {
        //     if (timer_elapsed32(last_keypress_timer) < 300) {
        //         process_right_thumb_magic(get_last_keycode(), get_last_mods()); set_last_keycode(KC_SPC);
        //         sym2_active = false;
        //     } else {
        //         if (record->event.pressed) {
        //             layer_on(_SYM2);
        //         } else {
        //             layer_off(_SYM2);
        //         }
        //     }
        //     return false;
        // }
        // intercepting mod taps: https://docs.qmk.fm/mod_tap#mod-tap
        case LCTL_T(NEXT_TAB): {
            if (record->tap.count && record->event.pressed) {
                // next tab
                register_code(KC_LCTL);
                tap_code16(KC_TAB);
                unregister_code(KC_LCTL);
                return false;
            }
            break;
        }
        // intercepting mod taps: https://docs.qmk.fm/mod_tap#mod-tap
        case LALT_T(ADDR_BAR): {
            if (record->tap.count && record->event.pressed) {
                // address bar
                register_code(KC_LALT);
                tap_code(KC_D);
                unregister_code(KC_LALT);
                return false;
            }
            break;
        }
        // intercepting mod taps: https://docs.qmk.fm/mod_tap#mod-tap
        case LGUI_T(PREV_TAB): {
            if (record->tap.count && record->event.pressed) {
                // previous tab
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                tap_code16(KC_TAB);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                return false;
            }
            break;
        }
        case LT(_FUN, KC_Q): {
            if (record->tap.count && record->event.pressed) {
                tap_code(KC_Q);
                tap_code(KC_U);
            } else if (!record->tap.count && record->event.pressed) {
                tap_code(KC_Q);
            }
            return false;
        }

    }
    switch (keycode) {
        case right_ret:
            if (record->event.pressed) {
                tap_code(KC_RIGHT);
                tap_code(KC_ENT);
            }
            return false;
        case line_begin_del:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                tap_code(KC_HOME);
                unregister_code(KC_LSFT);
                tap_code(KC_BSPC);
            }
            return false;
        case line_end_del:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                tap_code(KC_END);
                unregister_code(KC_LSFT);
                tap_code(KC_DEL);
            }
            return false;
        case dup_tab:
            if (record->event.pressed) {
                register_code(KC_LALT);
                tap_code(KC_D);
                unregister_code(KC_LALT);
                register_code(KC_LCTL);
                tap_code(KC_C);
                unregister_code(KC_LCTL);
                register_code(KC_LCTL);
                tap_code(KC_T);
                unregister_code(KC_LCTL);
                register_code(KC_LALT);
                tap_code(KC_D);
                unregister_code(KC_LALT);
                register_code(KC_LCTL);
                tap_code(KC_V);
                unregister_code(KC_LCTL);
                tap_code(KC_ENT);
            }
            return false;
        case CTRL_ALT_DEL:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LALT);
                tap_code(KC_DEL);
                unregister_code(KC_LALT);
                unregister_code(KC_LCTL);
            }
            return false;
    }

    update_swapper(
        &sw_win_active,
        KC_LALT,
        KC_TAB,
        SW_WIN,
        keycode,
        record
    );

    /*  If you want to block QMK from emitting the raw SW_WIN key,
    you can early-exit when it’s seen.  A compact way is:        */
    if (keycode == SW_WIN) return false;

    if (record->event.pressed) {
        if (keycode != OS_SHFT) os_shft_sent = false;
        if (keycode != OS_CTRL) os_ctrl_sent = false;
        if (keycode != OS_ALT)  os_alt_sent = false;
        if (keycode != OS_GUI)  os_gui_sent = false;
    }

    // Block if key manually handled
    if (update_oneshot(&os_shft_state, &os_shft_sent, KC_LSFT, OS_SHFT, keycode, record)) return false;
    if (update_oneshot(&os_ctrl_state, &os_ctrl_sent, KC_LCTL, OS_CTRL, keycode, record)) return false;
    if (update_oneshot(&os_alt_state,  &os_alt_sent,  KC_LALT, OS_ALT,  keycode, record)) return false;
    if (update_oneshot(&os_gui_state,  &os_gui_sent,  KC_LGUI, OS_GUI,  keycode, record)) return false;

    if (record->event.pressed && keycode != MAGIC_RI && keycode != MAGIC_CT && keycode != MAGIC_LT) {
        last_keypress_timer = timer_read32();  // Update AFTER processing the key
    }

    return true;
}

void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _NAV1));
    rgblight_set_layer_state(1, layer_state_cmp(state, _NAV2));
    rgblight_set_layer_state(2, layer_state_cmp(state, _SYM1));
    rgblight_set_layer_state(3, layer_state_cmp(state, _SYM2));
    rgblight_set_layer_state(4, layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(5, layer_state_cmp(state, _FUN));
    rgblight_set_layer_state(6, layer_state_cmp(state, _DEL));
    //rgblight_set_layer_state(7, layer_state_cmp(state, _MOU));
    return state;
}


#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
};
#endif
