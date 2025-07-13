/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

//#define USE_MATRIX_I2C

//#define QUICK_TAP_TERM 0
//#define TAPPING_TERM 100
#define COMBO_TERM 35
#define TAPPING_TERM 250
//#define TAPPING_TERM_PER_KEY
#define FLOW_TAP_TERM 200
#define LEADER_TIMEOUT 1250
#define LEADER_PER_KEY_TIMING
#define LEADER_KEY_STRICT_KEY_PROCESSING
#define RGBLIGHT_MAX_LAYERS 9
#define SPLIT_LAYER_STATE_ENABLE
#define RGBLIGHT_LAYERS
#define RGB_DI_PIN D3             // This is typical for Corne
#define SPLIT_TRANSPORT_MIRROR   // Optional: mirrors lighting effect to right half
#define RGBLIGHT_SPLIT
#undef RGBLED_SPLIT
#define RGBLED_SPLIT {24, 24}     // VERY important — tells QMK it’s a split layout!
// #define ONESHOT_TAP_TOGGLE 5  /* Tapping this number of times holds the key until tapped once again. */
// #define ONESHOT_TIMEOUT 3000  /* Time (in ms) before the one shot key is released */

#ifdef RGBLIGHT_ENABLE
    #define RGBLIGHT_EFFECT_STATIC_LIGHT
    //#define RGBLIGHT_EFFECT_BREATHING
    //#define RGBLIGHT_EFFECT_RAINBOW_MOOD
    //#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
    //#define RGBLIGHT_EFFECT_SNAKE
    //#define RGBLIGHT_EFFECT_KNIGHT
    //#define RGBLIGHT_EFFECT_CHRISTMAS
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
    //#define RGBLIGHT_EFFECT_RGB_TEST
    //#define RGBLIGHT_EFFECT_ALTERNATING
    //#define RGBLIGHT_EFFECT_TWINKLE
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
    #undef RGBLIGHT_ANIMATIONS
#endif
