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

## [Segment LCD](https://www.sensorwatch.net/docs/wig/display/)

- Positions 0-1 (weekday digits): watchface title
  - Limitations:
  - 0: any character in the Sensor Watch character set
  - 1: the letters A, B, C, D, E, F, H, I, J, L, N, O, R, T, U and X, and the numbers 0, 1, 3, 7 and 8
- Positions 2-3 (day digits): a single letter or a number from 0-39
  - Limitations:
  - 2: the numbers 1, 2 and 3
  - 3: any number and just about any letter
- Positions 4-9 (clock digits):
  - Limitations:
  - M’ and ‘W’ are not available, they will render as an ‘N’ or a ‘U’ respectively
  - some letters aren’t available in their preferred case (R -> r)
  - 4, 6: segments AD tied together -> unsupported F, K, P, Q, T, X and Y, as well as the underscore and a few other punctuation marks
- 5 Indicators (signal, bell, PM, 24H, LAP)
- Colon
- 3 buttons (Light, Mode, Alarm)
  - Light button illuminates the watch face (more brightly on the left side)
  - Mode button cycles through the watch modes (a press should resign immediately to the next watch face)
  - Alarm button is multifunctional
  - main interface should aim to present one screen of content with clarity
  - secondary screens can hide behind presses of Alarm or Light buttons
  -

## Movement (#define proposal_face)

- Weekday digits for watch face title ("Pr", "Hi", "Vy")
- Day digits for 30-second countdown
- Clock digits for "Will you marry me?"
- proposal_setup
- proposal_activate
- proposal_loop
- proposal_resign
- proposal_wants_background_task

## Constraints

## Useful Links

- [Online Firmware Builder](https://sensor-watch-builder.fly.dev/)
- [segmap.html](https://joeycastillo.github.io/Sensor-Watch-Documentation/segmap)
