# Required Libraries

The following Arduino libraries are required for this project.

## 1. AFMotor

- **Purpose:** Controls the DC motors through the L293D-based Adafruit Motor Shield.
- **Installation:** Install the `AFMotor` library compatible with your motor shield through the Arduino IDE Library Manager or the library's official source.

## 2. Servo

- **Purpose:** Controls the servo motor used to rotate the ultrasonic sensor for obstacle detection.
- **Installation:** Usually included with the Arduino AVR board package. Install or update the library if it is unavailable.

## Hardware Dependencies

- Arduino Uno
- L293D-based motor shield compatible with `AFMotor`
- HC-SR04 ultrasonic sensor
- Servo motor
- HC-05 Bluetooth module (for Bluetooth control)

**Note:** Voice recognition is handled by an external app that sends commands to the Arduino over serial Bluetooth. It does not require a separate speech-recognition library in this sketch.
