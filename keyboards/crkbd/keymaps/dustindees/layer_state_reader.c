#include <stdio.h>
#include "action_layer.h"

// in the future, should use (1U<<_LAYER_NAME) instead, but needs to be moved to keymap,c
// #define L_BASE 0
// #define L_LOWER 2
// #define L_RAISE 4
// #define L_ADJUST 8
// #define L_ADJUST_TRI 14

char layer_state_str[24];

const char *read_layer_state(void) {
    switch (get_highest_layer(layer_state)) {
        case 0:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: DEF");
            break;
        case 1:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: NAV1");
            break;
        case 2:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: NAV2");
            break;
        case 3:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: SYM1");
            break;
        case 4:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: SYM2");
            break;
        case 5:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: NUM");
            break;
        case 6:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: FUN");
            break;
        case 7:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: DEL");
            break;
        case 8:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: MOU");
            break;
        default:
            snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Unknown");
    }
    return layer_state_str;
}

// const char *read_layer_state(void) {
//   switch (layer_state)
//   {
//   case L_BASE:
//     snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Default");
//     break;
//   case L_RAISE:
//     snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Raise");
//     break;
//   case L_LOWER:
//     snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Lower");
//     break;
//   case L_ADJUST:
//   case L_ADJUST_TRI:
//     snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Adjust");
//     break;
//   default:
// #if defined (LAYER_STATE_32BIT)
//     snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
// #else
//     snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%d", layer_state);
// #endif
//   }

//   return layer_state_str;
// }
