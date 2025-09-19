#include QMK_KEYBOARD_H
#include "version.h"
#include "i18n.h"

#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

// --- Snap Tap (SOCD) toggle state ---
static uint16_t led_blink_timer = 0;
static bool leds_on = false;

// SOCD Cleaner configuration
socd_cleaner_t socd_opposing_pairs[] = {
  {{KC_S, KC_F}, SOCD_CLEANER_LAST},
};


enum custom_keycodes {
  RGB_SLD = ZSA_SAFE_RANGE,
  ST_MACRO_0,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           RCTL(KC_F5),                                    KC_CAPS,        KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_F12,
    KC_TAB,         KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_MEH,                                         KC_HYPR,        KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           NO_AM,
    KC_LEFT_SHIFT,  KC_A,           KC_S,           KC_D,           KC_F,           KC_G,                                                                           KC_H,           KC_J,           KC_K,           KC_L,           NO_AE,          MT(MOD_RSFT, NO_OSLH),
    MO(3),          KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,           KC_F9,                                          KC_MEH,         KC_N,           KC_M,           KC_COMMA,       KC_DOT,         NO_MINS,        MO(3),
    KC_LEFT_CTRL,   KC_F13,         KC_LEFT_ALT,    KC_LEFT_GUI,    MO(2),                                                                                                          MO(2),          MO(1),          KC_APPLICATION, KC_LEFT_ALT,    TT(1),
                                                                                                    KC_AUDIO_VOL_DOWN,KC_AUDIO_VOL_UP,KC_MEDIA_PREV_TRACK,KC_MEDIA_NEXT_TRACK,
                                                                                                                    KC_AUDIO_MUTE,  KC_MEDIA_PLAY_PAUSE,
                                                                                    KC_SPACE,       KC_ENTER,       KC_BSPC,        KC_ESCAPE,      KC_DELETE,      KC_BSPC
  ),
  [1] = LAYOUT_ergodox_pretty(
    KC_TRANSPARENT, KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, NO_SLSH,        KC_1,           KC_2,           KC_3,           KC_KP_PLUS,     KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                 KC_PERC,        KC_4,           KC_5,           KC_6,           NO_EQL,         KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, LSFT(KC_BSLS),  KC_7,           KC_8,           KC_9,           NO_MINS,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_0,           KC_TRANSPARENT, KC_KP_COMMA,    KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [2] = LAYOUT_ergodox_pretty(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_MS_BTN3,     KC_MS_BTN1,     KC_MS_UP,       KC_MS_BTN2,     KC_MS_WH_UP,    KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_WWW_FORWARD, KC_HOME,        KC_UP,          KC_PAGE_UP,     KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_MS_ACCEL2,   KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_MS_WH_DOWN,                                                                  KC_WWW_BACK,    KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_MS_ACCEL2,   KC_TRANSPARENT,
    KC_TRANSPARENT, KC_MS_ACCEL0,   KC_TRANSPARENT, LGUI(LCTL(KC_LEFT)),LGUI(LCTL(KC_RIGHT)),KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_END,         KC_DOWN,        KC_PGDN,        KC_MS_ACCEL0,   KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [3] = LAYOUT_ergodox_pretty(
    DM_REC1,        KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_F6,                                          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,         ST_MACRO_0,
    DM_RSTP,        KC_HASH,        NO_AT,          RALT(KC_4),     LSFT(KC_EQUAL), LSFT(KC_BSLS),  NO_CIRC,                                        KC_TRANSPARENT, NO_SLSH,        NO_LBRC,        NO_RBRC,        NO_TILD,        NO_PLUS,        KC_TRANSPARENT,
    DM_PLY1,        NO_AMPR,        NO_SLSH,        KC_EQUAL,       LSFT(KC_2),     NO_QUES,                                                                        KC_PERC,        NO_LPRN,        NO_RPRN,        NO_QUES,        NO_EQL,         KC_TRANSPARENT,
    KC_TRANSPARENT, KC_GRAVE,       NO_LESS,        NO_GRTR,        KC_BSLS,        KC_EXLM,        RALT(KC_5),                                     KC_TRANSPARENT, LSFT(KC_BSLS),  RALT(KC_7),     RALT(KC_0),     KC_EXLM,        NO_UNDS,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                    LCTL(KC_INSERT),LSFT(KC_INSERT),KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_DELETE,      KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
};



// ------------------------ Process Record ------------------------
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // --- Ctrl + Caps = toggle Snap Tap ---
    // Check if any Ctrl key is currently held down
    if (keycode == KC_CAPS && record->event.pressed) {
        // Check if Ctrl is currently pressed by reading the modifier state
        if (get_mods() & MOD_MASK_CTRL) {
            socd_cleaner_enabled = !socd_cleaner_enabled;
            led_blink_timer = timer_read();

            // Turn off LEDs if Snap Tap disabled
            if (!socd_cleaner_enabled) {
                ergodox_right_led_1_off();
                ergodox_right_led_2_off();
                ergodox_right_led_3_off();
            }
            return false; // block normal Caps behavior
        }
    }

    // --- Custom macro ---
    switch (keycode) {
        case ST_MACRO_0:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_F4)));
            }
            return false;
    }

    return true; // default behavior
}

// ------------------------ LED Blink for Snap Tap ------------------------
void matrix_scan_user(void) {
    if (!socd_cleaner_enabled) return;

    // Toggle LEDs every 300 ms
    if (timer_elapsed(led_blink_timer) > 300) {
        leds_on = !leds_on;
        led_blink_timer = timer_read();

        if (leds_on) {
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
        } else {
            ergodox_right_led_1_off();
            ergodox_right_led_2_off();
            ergodox_right_led_3_off();
        }
    }
}

// ------------------------ Layer LED Handling ------------------------
uint8_t layer_state_set_user(uint8_t state) {
    if (socd_cleaner_enabled) {
        // Do not show layer LEDs while Snap Tap active
        return state;
    }

    uint8_t layer = biton(state);
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    switch (layer) {
        case 1: ergodox_right_led_1_on(); break;
        case 2: ergodox_right_led_2_on(); break;
        case 3: ergodox_right_led_3_on(); break;
        case 4: ergodox_right_led_1_on(); ergodox_right_led_2_on(); break;
        case 5: ergodox_right_led_1_on(); ergodox_right_led_3_on(); break;
        case 6: ergodox_right_led_2_on(); ergodox_right_led_3_on(); break;
        case 7: ergodox_right_led_1_on(); ergodox_right_led_2_on(); ergodox_right_led_3_on(); break;
        default: break;
    }

    return state;
}