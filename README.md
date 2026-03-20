# Wabot

Wabot is an Arduino-based autonomous robot that:

- Uses 3 ultrasonic sensors to avoid obstacles.
- Uses an AS726x color sensor to detect red targets.
- Activates a pump relay when red is detected.

Project source files are in [Wabot/](Wabot/).

## Features

- Modular code split into:
  - `DriveSystem` (motor control and navigation decisions)
  - `UltrasonicSensors` (distance acquisition + median filtering)
  - `ColourSensor` (red detection from spectral channels)
  - `Pump` (relay control)
- Obstacle-aware steering from left/center/right distance readings.
- Stop-and-spray behavior when a red object is identified.

## Repository Structure

- `Wabot/Wabot.ino` - Arduino sketch entry point (`setup()` / `loop()`).
- `Wabot/DriveSystem.h`, `Wabot/DriveSystem.cpp` - drivetrain and navigation logic.
- `Wabot/UltrasonicSensors.h`, `Wabot/UltrasonicSensors.cpp` - 3 ultrasonic sensors and filtering.
- `Wabot/ColourSensor.h`, `Wabot/ColourSensor.cpp` - AS726x setup and red detection logic.
- `Wabot/Pump.h`, `Wabot/Pump.cpp` - pump relay abstraction.

## Hardware Assumptions

The current pin mapping in code is:

### Ultrasonic Sensors

- Left: trig `8`, echo `9`
- Center: trig `7`, echo `6`
- Right: trig `4`, echo `5`

### Drive System

Constructed as:

```cpp
DriveSystem drive(3, 2, 12, 11, 13, A3);
```

This maps to:

- Right motor: `enR=3`, `r1=2`, `r2=12`
- Left motor: `enL=11`, `l1=13`, `l2=A3`

### Pump

- Relay control pin: `10`

## Software Dependencies

- Arduino IDE (or Arduino CLI)
- Board support for your Arduino target
- `Adafruit AS726x` library

Install the library in Arduino IDE:

1. Open **Library Manager**.
2. Search for **Adafruit AS726x**.
3. Install the latest version.

## Build and Upload

1. Open `Wabot/Wabot.ino` in Arduino IDE.
2. Select your board and serial port.
3. Verify/compile.
4. Upload.

## Runtime Behavior

In each loop cycle:

1. Distance sensors are sampled and median-filtered.
2. Drive logic chooses motion based on nearby obstacles.
3. Color sensor checks whether red is detected.
4. If red is detected:
   - Robot stops.
   - Pump turns on.
   - Program enters an infinite loop to keep spraying.

## Notes

- A distance value of `-1` means ultrasonic timeout/no echo.
- Drive logic uses short blocking `delay(...)` calls in navigation.
- Pump is switched by writing HIGH/LOW to the relay pin; if your relay is active-low, invert the logic in `Pump.cpp`.

## Future Improvements

- Replace blocking delays with non-blocking timing (`millis()`).
- Add tunable thresholds and calibration constants.
- Add a safety timeout for pump runtime.
- Add serial debug mode flags for cleaner logs.
