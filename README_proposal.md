# Proposal Watch Face

The Proposal is a special watch face that displays "Will you marry me?" or whatever phrase you desire, triggered by a special button press combination. I designed it to surprise my girlfriend. We'll see how it goes...

## TODOs:
[ ] Design "Will you marry me?" for segment LCD using emulator (10 char limit, hide additional screens behind Light or Alarm button) 
[ ] Evaluate how words are spelled with tricky letters in other watch faces (Activity face, Tarot face)
[ ] Implement button combination trigger
[ ] Add extra easter eggs (Yes/No screen, animations like Randonaut, buzzer, indicators)
[ ] Work gracefully with Movement’s low energy mode
[ ] Set custom timeout setting to reset back to first screen
[ ] Add other useful watch faces (simple_clock, world_clock, preferences, set_time, stopwatch, voltage, sunrise_sunset, countdown, counter, nanosec, finetune, tossup, proposal, 8ball)

## Segment LCD
- https://www.sensorwatch.net/docs/wig/display/
- https://sensor-watch-builder.fly.dev/
- Weekday digits for watch face title ("Pr", "Hi")
- Day digits for 30-second countdown
- Clock digits, the following characters are fully unsupported in positions 4 and 6: F, K, P, Q, T, X and Y
- Indicators (signal, bell)

## Movement (#define proposal_face)
- proposal_setup
- proposal_activate
- proposal_loop
- proposal_resign
- proposal_wants_background_task

