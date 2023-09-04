/*
 * MIT License
 *
 * Copyright (c) 2023 <Hieu Nguyen>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include "proposal_face.h"

#define PROPOSAL_WATCHFACE_TITLE "  "
#define ANIMATION_TICK_FREQ_NAME 2
#define ANIMATION_TICK_FREQ_PROPOSAL 0.5
#define ANIMATION_TICK_FREQ_RESPONSE 2
#define ANIMATION_TICK_FREQ_REACTION 2
#define MAX_NAME_COUNT 3
#define STATE_NAME 0
#define STATE_PROPOSAL 1
#define STATE_RESPONSE 2
#define STATE_REACTION 3

#define WORD_DEATH " Death"
#define WORD_EGGHEAD "EggHed"
#define WORD_HIEU " HiEU "
#define WORD_LOVERS "Lovers"
#define WORD_VY "  vy  "

// TODO: For some reason, switching between words[] drops the first frame... so adding an extra one to compensate

static char words_name[][7] = {
    "vy    ",
    WORD_VY,
};

static char words_proposal[][7] = {
    "W  LL ", // W is only top half left -> complete with watch_set_pixel()
    "W  LL ",
    "   You",
    "n&arry",
    "n&E ? ",
};

static char words_response[][7] = {
    " Y  no",
    " Y  no",
};

static char words_reaction_yes[][7] = {
    WORD_LOVERS, // with GREEN light
    WORD_LOVERS,
    WORD_HIEU,
    WORD_VY,
};

static char words_reaction_no[][7] = {
    WORD_DEATH, // with RED light
    WORD_DEATH,
    "  2   ",
    WORD_EGGHEAD,
};

#define NUM_WORDS_NAME (sizeof(words_name) / sizeof(*words_name))
#define NUM_WORDS_PROPOSAL (sizeof(words_proposal) / sizeof(*words_proposal))
#define NUM_WORDS_RESPONSE (sizeof(words_response) / sizeof(*words_response))
#define NUM_WORDS_REACTION_Y (sizeof(words_reaction_yes) / sizeof(*words_reaction_yes))
#define NUM_WORDS_REACTION_N (sizeof(words_reaction_no) / sizeof(*words_reaction_no))

void proposal_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(proposal_state_t));
        memset(*context_ptr, 0, sizeof(proposal_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void proposal_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    proposal_state_t *state = (proposal_state_t *)context;

    // Handle any tasks related to your watch face coming on screen.
    // watch_display_string(PROPOSAL_WATCHFACE_TITLE, 0);
    movement_request_tick_frequency(ANIMATION_TICK_FREQ_NAME);
    state->state = 0;
    state->word_index = 0;
    state->name_count = 0;
    state->proposal_response = false;
}

static void _proposal_face_update_lcd(proposal_state_t *state) {
    char buf[11];
    switch (state->state) {
        case STATE_NAME:
            sprintf(buf, "    %s", words_name[state->word_index]);
            break;
        case STATE_PROPOSAL:
            sprintf(buf, "    %s", words_proposal[state->word_index]);
            break;
        case STATE_RESPONSE:
            sprintf(buf, "    %s", words_response[state->word_index]);
            break;
        case STATE_REACTION:
            if (state->proposal_response) {
                sprintf(buf, "    %s", words_reaction_yes[state->word_index]);
            } else {
                sprintf(buf, "    %s", words_reaction_no[state->word_index]);
            }
            break;
        default:
            sprintf(buf, "    %s", words_name[state->word_index]);
            break;
    }

    watch_display_string(buf, 0);

    // Modify display string with extra segments for "WiLL"
    if (state->state == STATE_PROPOSAL && state->word_index < 2) {
        watch_set_pixel(1, 20); // partial upper right half of W
        watch_set_pixel(2, 21); // partial upper right half of W
        watch_set_pixel(1, 22); // i
    }
}

// Play one-up sound effect from Mario
void beep_happy (void) {
        const BuzzerNote notes[] = {
            BUZZER_NOTE_E5,
            BUZZER_NOTE_G5,
            BUZZER_NOTE_E6,
            BUZZER_NOTE_C6,
            BUZZER_NOTE_D6,
            BUZZER_NOTE_G6,
        };
        const uint16_t durations[] = {
            150,
            150,
            150,
            150,
            150,
            150,
        };
        for(size_t i = 0, count = sizeof(notes) / sizeof(notes[0]); i < count; i++) {
            watch_buzzer_play_note(notes[i], durations[i]);
        }
}

// Play death sound effect from Mario
void beep_sad (void) {
        const BuzzerNote notes[] = {
            BUZZER_NOTE_C5,
            BUZZER_NOTE_C5SHARP_D5FLAT,
            BUZZER_NOTE_D5,
            BUZZER_NOTE_REST,
            BUZZER_NOTE_B4,
            BUZZER_NOTE_F5,
            BUZZER_NOTE_REST,
            BUZZER_NOTE_F5,
            BUZZER_NOTE_REST,
            BUZZER_NOTE_F5,
            BUZZER_NOTE_E5,
            BUZZER_NOTE_D5,
            BUZZER_NOTE_C5,
            BUZZER_NOTE_E4,
            BUZZER_NOTE_REST,
            BUZZER_NOTE_E4,
            BUZZER_NOTE_C4,
        };
        const uint16_t durations[] = {
            40,
            40,
            80,
            600,
            150,
            150,
            150,
            150,
            25,
            250,
            250,
            250,
            150,
            150,
            150,
            150,
            150,
        };
        for(size_t i = 0, count = sizeof(notes) / sizeof(notes[0]); i < count; i++) {
            watch_buzzer_play_note(notes[i], durations[i]);
        }
}

bool proposal_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    proposal_state_t *state = (proposal_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            // Show your initial UI here.
            _proposal_face_update_lcd(state);
            break;
        case EVENT_TICK:
            // If needed, update your display here.
            // if (state->active) {
            //     if (event.subsecond % 2 == 0) watch_set_led_off();
            //     else if (state->color == 0) watch_set_led_red();
            //     else if (state->color == 1) watch_set_led_green();
            //     else watch_set_led_yellow();
            // }

            // State management
            switch (state->state) {
                case STATE_NAME:
                    if (state->name_count < MAX_NAME_COUNT) {
                        state->word_index = (state->word_index + 1) % NUM_WORDS_NAME;
                        _proposal_face_update_lcd(state);

                        if (state->word_index == NUM_WORDS_NAME - 1) {
                            state->name_count++;
                        }
                    } else {
                        movement_request_tick_frequency(ANIMATION_TICK_FREQ_PROPOSAL);
                        state->state = STATE_PROPOSAL;
                        state->word_index = 0;
                    }
                    break;
                case STATE_PROPOSAL:
                    if (state->word_index < NUM_WORDS_PROPOSAL - 1) {
                        state->word_index = (state->word_index + 1) % NUM_WORDS_PROPOSAL;
                        _proposal_face_update_lcd(state);
                    } else {
                        movement_request_tick_frequency(ANIMATION_TICK_FREQ_RESPONSE);
                        state->state = STATE_RESPONSE;
                        state->word_index = 0;
                    }
                    break;
                case STATE_RESPONSE:
                    state->word_index = (state->word_index + 1) % NUM_WORDS_RESPONSE;
                    _proposal_face_update_lcd(state);

                    // Toggle bell indicator
                    if (state->word_index == 0) {
                        watch_set_indicator(WATCH_INDICATOR_BELL);
                    } else {
                        watch_clear_indicator(WATCH_INDICATOR_BELL);
                    }
                    break;
                case STATE_REACTION:
                    if (state->proposal_response) {
                        state->word_index = (state->word_index + 1) % NUM_WORDS_REACTION_Y;
                    } else {
                        state->word_index = (state->word_index + 1) % NUM_WORDS_REACTION_N;
                    }
                    _proposal_face_update_lcd(state);

                    // Blink LED every 5 frames
                    if (state->word_index % 5 == 0) {
                        if (state->proposal_response) {
                            watch_set_led_green();
                            movement_illuminate_led();
                            beep_happy();
                        } else {
                            watch_set_led_red();
                            movement_illuminate_led();
                            beep_sad();
                        }
                    } else {
                        watch_set_led_off();
                    }
                    
                    break;
                default:
                    break;
            }
            break;
        case EVENT_LIGHT_BUTTON_DOWN:
            // You can use the Light button for your own purposes. Note that by default, Movement will also
            // illuminate the LED in response to EVENT_LIGHT_BUTTON_DOWN; to suppress that behavior, add an
            // empty case for EVENT_LIGHT_BUTTON_DOWN.

            // Only functional for YES response
            if (state->state == STATE_RESPONSE) {
                state->proposal_response = true;
                movement_request_tick_frequency(ANIMATION_TICK_FREQ_REACTION);
                state->state = STATE_REACTION;
                state->word_index = 0;
            }
            break;
        case EVENT_ALARM_BUTTON_DOWN:
            // Only functional for NO response
            if (state->state == STATE_RESPONSE) {
                state->proposal_response = false;
                movement_request_tick_frequency(ANIMATION_TICK_FREQ_REACTION);
                state->state = STATE_REACTION;
                state->word_index = 0;
            }
            break;
        case EVENT_ALARM_LONG_PRESS:
            // Reset to name state
            watch_set_led_off();
            movement_request_tick_frequency(ANIMATION_TICK_FREQ_NAME);
            state->state = STATE_NAME;
            state->word_index = 0;
            state->name_count = 0;
            break;
        case EVENT_TIMEOUT:
            // Your watch face will receive this event after a period of inactivity. If it makes sense to resign,
            // you may uncomment this line to move back to the first watch face in the list:
            movement_move_to_face(0);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // If you did not resign in EVENT_TIMEOUT, you can use this event to update the display once a minute.
            // Avoid displaying fast-updating values like seconds, since the display won't update again for 60 seconds.
            // You should also consider starting the tick animation, to show the wearer that this is sleep mode:
            // watch_start_tick_animation(500);
            break;
        default:
            // Movement's default loop handler will step in for any cases you don't handle above:
            // * EVENT_LIGHT_BUTTON_DOWN lights the LED
            // * EVENT_MODE_BUTTON_UP moves to the next watch face in the list
            // * EVENT_MODE_LONG_PRESS returns to the first watch face (or skips to the secondary watch face, if configured)
            // You can override any of these behaviors by adding a case for these events to this switch statement.
            return movement_default_loop_handler(event, settings);
    }

    // return true if the watch can enter standby mode. Generally speaking, you should always return true.
    // Exceptions:
    //  * If you are displaying a color using the low-level watch_set_led_color function, you should return false.
    //  * If you are sounding the buzzer using the low-level watch_set_buzzer_on function, you should return false.
    // Note that if you are driving the LED or buzzer using Movement functions like movement_illuminate_led or
    // movement_play_alarm, you can still return true. This guidance only applies to the low-level watch_ functions.
    return true;
}

void proposal_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;

    // handle any cleanup before your watch face goes off-screen.
    watch_set_led_off();
}
