# Wabot

Wabot is an Arduino-based autonomous robot designed for obstacle avoidance and navigation, "fire" (red) detection, and targeted water spraying. It integrates ultrasonic sensors for navigation, a color sensor for red detection, and a pump system to activate upon identifying red targets.

## Features

- **Obstacle Avoidance**: Utilizes three ultrasonic sensors (left, center, right) to detect and navigate around obstacles.
- **Red Detection**: Uses an AS726x color sensor to identify red objects based on spectral analysis.
- **Pump Control**: Activates a water pump via relay when red is detected, with a stop-and-spray behavior.
- **Modular Design**: Code is organized into separate classes for maintainability:
  - `DriveSystem`: Motor control and navigation logic.
  - `UltrasonicSensors`: Obstacle distance measurement with median filtering for accuracy.
  - `ColourSensor`: Spectral analysis for red detection.
  - `Pump`: Relay-based pump control.
- **Differential Drive**: Supports forward movement, pivoting turns, and obstacle avoidance maneuvers.

## Repository Structure

- `Wabot/Wabot.ino` - Main Arduino sketch containing `setup()` and `loop()` functions.
- `Wabot/DriveSystem.h` / `Wabot/DriveSystem.cpp` - Differential drive motor control and navigation decisions.
- `Wabot/UltrasonicSensors.h` / `Wabot/UltrasonicSensors.cpp` - Ultrasonic sensor management with median filtering.
- `Wabot/ColourSensor.h` / `Wabot/ColourSensor.cpp` - AS726x color sensor setup and red detection logic.
- `Wabot/Pump.h` / `Wabot/Pump.cpp` - Pump relay abstraction.

## Hardware Requirements

### Components
- Arduino board (e.g., Arduino Uno)
- 3x HC-SR04 ultrasonic sensors
- Adafruit AS726x color sensor
- 2x DC motors with L298N motor driver
- 5V Relay module for pump control
- 3V Submersible water pump
- 12V-5A AC-DC Power supply
- Jumper wires and robot chassis

### Pin Mapping

#### Ultrasonic Sensors
- Left: Trig pin 8, Echo pin 9
- Center: Trig pin 7, Echo pin 6
- Right: Trig pin 4, Echo pin 5

#### Drive System
```cpp
DriveSystem drive(3, 2, 12, 11, 13, A3);
```
- Right motor: Enable 3, Input1 2, Input2 12
- Left motor: Enable 11, Input1 13, Input2 A3

#### Pump
- Relay control pin: 10

#### Colour Sensor
- I2C: SDA A4, SCL A5 (default for Arduino Uno)

## Software Dependencies

- Arduino IDE or Arduino CLI
- Adafruit AS726x library

### Installing the Library
1. Open Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries**.
3. Search for "Adafruit AS726x".
4. Install the latest version.

## Installation and Setup

1. Clone or download this repository.
2. Open `Wabot/Wabot.ino` in Arduino IDE.
3. Ensure the correct board and port are selected.
4. Install the required library as described above.
5. Verify and upload the sketch to your Arduino board.

## Usage

### Runtime Behavior

The robot operates in a continuous loop:

1. **Sensor Update**: Reads distances from ultrasonic sensors with median filtering for accuracy.
2. **Navigation**: Decides movement based on obstacle proximity:
   - Moves forward if no obstacles.
   - Turns left/right for side obstacles.
   - Executes avoidance maneuvers for center obstacles.
3. **Color Detection**: Checks for red using spectral channels.
4. **Pump Activation**: If red is detected:
   - Stops movement.
   - Turns on the pump.
   - Waits until red is no longer detected, then turns off pump and halts.

### Key Parameters

- **Obstacle Threshold**: 20 cm (defined in `DriveSystem.h`)
- **Main Loop Delay**: 500 ms (adjustable in `Wabot.ino`)
- **Motor Speed**: Initial 200 (0-255 range, adjustable via `drive.setSpeed()`)

### Code Overview

#### Main Loop (`Wabot.ino`)
```cpp
void loop() {
    ultrasonic.update();
    drive.navigate(ultrasonic.readings);

    bool status = colour.redDetected();
    if (status) {
        drive.stop();
        pump.on();
        while (colour.redDetected()) {
            delay(10);
        }
        pump.off();
        while (true) {
            drive.stop();
        }
    } else {
        pump.off();
    }
    delay(mainLoopDelayMs);
}
```

#### Navigation Logic (`DriveSystem.cpp`)
The `navigate()` function uses a state-based approach:
- State 0: Clear path → move forward
- State 1-2: Side obstacles → turn away
- State 3-7: Center obstacles → complex avoidance maneuvers

#### Red Detection (`ColourSensor.cpp`)
Compares red and orange channels against violet, blue, green, and yellow for dominance.

## Troubleshooting

- **Ultrasonic Sensors**: -1 readings indicate timeout; check wiring and power.
- **Color Sensor**: "Sensor not found" error; verify I2C connections and library installation.
- **Motors Not Moving**: Check motor driver wiring and power supply.
- **Pump Not Activating**: Verify relay pin and logic (active HIGH assumed).

## Future Improvements

- Replace blocking delays with non-blocking `millis()`-based timing.
- Add configurable thresholds via serial input.
- Implement safety timeouts for pump operation.
- Add debug modes with detailed serial logging.
- Integrate PID control for smoother navigation.
- Add battery voltage monitoring.