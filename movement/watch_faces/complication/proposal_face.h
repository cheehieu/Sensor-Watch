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

#ifndef PROPOSAL_FACE_H_
#define PROPOSAL_FACE_H_

#include "movement.h"

/*
 * A DESCRIPTION OF YOUR WATCH FACE
 *
 * and a description of how use it
 *
 * Display the proposal
 * Auto change to next face (word_delay_ticks, animation delay after Vy...)
 * Answer yes or no (buttons)
 * Easter eggs (lights, buzzer, interactions, indicators)
 * 
 */

// TODO: secret access with long press of buttons (long press of Mode, then 3 short presses)
// TODO: auto scroll of words
// TODO: Yes/No responses (lights, animation)

typedef struct {
    bool proposal_response; // true: yes, false: no
    uint8_t name_count; // specifies how many times to animate name
    uint8_t state; // 0: name, 1: proposal, 2: response, 3: reaction
    uint8_t word_index; // keeps track of which word to display
} proposal_state_t;

void proposal_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr);
void proposal_face_activate(movement_settings_t *settings, void *context);
bool proposal_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void proposal_face_resign(movement_settings_t *settings, void *context);

#define proposal_face ((const watch_face_t){ \
    proposal_face_setup, \
    proposal_face_activate, \
    proposal_face_loop, \
    proposal_face_resign, \
    NULL, \
})

#endif // PROPOSAL_FACE_H_

