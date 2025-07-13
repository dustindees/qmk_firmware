// Taken from here: https://github.com/callum-oakley/qmk_firmware/blob/d63988caaf86217cfddf903c2c32dc042a8ef4da/users/callum/oneshot.h
#pragma once

#include QMK_KEYBOARD_H

// Represents the four states a oneshot key can be in
typedef enum {
    // Starting state indicating that the modifier isn't in a state to be
    // triggered.
    os_up_unqueued,
    // This is when you press and release the mod without having pressed another
    // key. It queues the modifier so that the next key you press will be
    // affected by the modifier.
    os_up_queued,
    // This indicates that the modifier is currently held down, but no key has
    // been modified by it yet.
    os_down_unused,
    // This indicates that the modifier is currently held down and has already
    // modified another key.
    os_down_used,
} oneshot_state;

// Custom oneshot mod implementation that doesn't rely on timers. If a mod is
// used while it is held it will be unregistered on keyup as normal, otherwise
// it will be queued and only released after the next non-mod keydown.
//
// `sent_keycode` is set to `true` if we sent a modified key, that way we don't
// end up repeating that key.
//void update_oneshot(oneshot_state *state, bool *sent_keycode, uint16_t mod, uint16_t trigger, uint16_t keycode, keyrecord_t *record);

bool update_oneshot(oneshot_state *state, bool *sent_keycode, uint16_t mod, uint16_t trigger, uint16_t keycode, keyrecord_t *record);


// void update_oneshot(
//     oneshot_state *state,
//     uint16_t mod,
//     uint16_t trigger,
//     uint16_t keycode,
//     keyrecord_t *record
// );

// To be implemented by the consumer. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// To be implemented by the consumer. Defines keys to ignore when determining
// whether a oneshot mod has been used. Setting this to modifiers and layer
// change keys allows stacking multiple oneshot modifiers, and carrying them
// between layers.
bool is_oneshot_ignored_key(uint16_t keycode);
