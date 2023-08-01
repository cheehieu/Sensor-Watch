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
#define PROPOSAL_ANIMATION_TICK_FREQUENCY 8
#define WORD_LOVERS "Lovers"
#define WORD_DEATH "Death"
#define WORD_EGGHEAD "EggHed"
#define WORD_HIEU " HiEU "

static char proposal_words[][7] = {
    "  vy  ",
    "vy    ",
    "W  LL ", // W is only top half left, complete with watch_set_pixel()
    "   You",
    "n&arry",
    "n&E ? ",
    " Y  no",
};

#define NUM_PROPOSAL_WORDS (sizeof(proposal_words) / sizeof(*proposal_words))

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
    state->word_index = 0;
}

static void _proposal_face_update_lcd(proposal_state_t *state) {
    char buf[11];
    sprintf(buf, "    %s", proposal_words[state->word_index]);
    watch_display_string(buf, 0);

    // Modify display string with extra segments
    if (state->word_index == 2) {
        watch_set_pixel(1, 20); // partial upper right half of W
        watch_set_pixel(2, 21); // partial upper right half of W
        watch_set_pixel(1, 22); // i
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
            break;
        case EVENT_LIGHT_BUTTON_UP:
            break;
        case EVENT_LIGHT_LONG_PRESS:
            // You can use the Light button for your own purposes. Note that by default, Movement will also
            // illuminate the LED in response to EVENT_LIGHT_BUTTON_DOWN; to suppress that behavior, add an
            // empty case for EVENT_LIGHT_BUTTON_DOWN.
            state->word_index = (state->word_index - 1) % NUM_PROPOSAL_WORDS;
            _proposal_face_update_lcd(state);
            break;
        case EVENT_ALARM_BUTTON_UP:
            // Just in case you have need for another button.
            state->word_index = (state->word_index + 1) % NUM_PROPOSAL_WORDS;
            _proposal_face_update_lcd(state);
            break;
        case EVENT_ALARM_LONG_PRESS:
            break;
        case EVENT_TIMEOUT:
            // Your watch face will receive this event after a period of inactivity. If it makes sense to resign,
            // you may uncomment this line to move back to the first watch face in the list:
            // movement_move_to_face(0);
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
    // watch_set_led_off();
}
