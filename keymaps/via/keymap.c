// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    KC_BL_TOGG = SAFE_RANGE,
    KC_BL_ALL_TOGG
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * +---------------------------+
     * │ L1│ L2│ L3│ L4│ L5| L6| L7|
     * +---------------------------+
     * │ F1 | F2 | F3 | F4 | F5|
     * +-----------------------+
     * │ F6 | F7 | F8 | F9 |F10|
     * +-----------------------+
     */
    [0] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_BL_TOGG, KC_BL_ALL_TOGG, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    ),
    [1] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    ),
    [2] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    ),
    [3] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    ),
    [4] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    ),
    [5] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    ),
    [6] = LAYOUT(
        TO(0), TO(1), TO(2), TO(3), TO(4), TO(5), TO(6),
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10
    )
};

#define BACKLIGHT_PINS_LENGTH 7
const uint8_t backlight_pins[BACKLIGHT_PINS_LENGTH] = { F4, F5, F6, F7, B1, B3, B2 };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool backlight_enabled = true;      // KC_BL_TOGG用
    static bool all_backlight_enabled = false; // KC_BL_ALL_TOGG用
    uint8_t current_layer = get_highest_layer(layer_state); // 現在のレイヤーを取得

    if (!all_backlight_enabled) {
        // すべてのLEDを消灯
        for (uint8_t i = 0; i < BACKLIGHT_PINS_LENGTH; i++) {
            writePinLow(backlight_pins[i]);
        }
        writePinHigh(D5);
        writePinHigh(B0);
        // 対応するLEDを点灯
        if (backlight_enabled) {
            if (current_layer < BACKLIGHT_PINS_LENGTH) {
                writePinHigh(backlight_pins[current_layer]);
            } else {
                // 範囲外のレイヤーなら全て消灯
                for (uint8_t i = 0; i < BACKLIGHT_PINS_LENGTH; i++) {
                    writePinLow(backlight_pins[i]);
                }
            }
        }
    }

    switch (keycode) {
        case KC_BL_TOGG:
            if (record->event.pressed) {
                backlight_enabled = !backlight_enabled; // バックライト状態をトグル
                if (!backlight_enabled) {
                    for (uint8_t i = 0; i < BACKLIGHT_PINS_LENGTH; i++) {
                        writePinLow(backlight_pins[i]);
                    }
                } else {
                    for (uint8_t i = 0; i < BACKLIGHT_PINS_LENGTH; i++) {
                        if (i == current_layer) {
                            writePinHigh(backlight_pins[i]);
                        } else {
                            writePinLow(backlight_pins[i]);
                        }
                    }
                }
            }
            return false;

        case KC_BL_ALL_TOGG:
            if (record->event.pressed) {
                all_backlight_enabled = !all_backlight_enabled;
                if (all_backlight_enabled) {
                    // すべてのバックライトLEDを点灯
                    for (uint8_t i = 0; i < BACKLIGHT_PINS_LENGTH; i++) {
                        writePinHigh(backlight_pins[i]);
                    }
                    // ProMicroのオンボードLEDも点灯
                    setPinOutput(D5);
                    setPinOutput(B0);
                    writePinLow(D5);
                    writePinLow(B0);
                } else {
                    // すべてのバックライトLEDを消灯
                    for (uint8_t i = 0; i < BACKLIGHT_PINS_LENGTH; i++) {
                        writePinLow(backlight_pins[i]);
                    }
                    // ProMicroのオンボードLEDも消灯
                    writePinHigh(D5);
                    writePinHigh(B0);
                }
            }
            return false;
    }
    return true;
}
