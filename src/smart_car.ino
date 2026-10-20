
/*
 * Obstacle-Avoiding, Bluetooth-Controlled and Voice-Controlled Robot Car
 *
 * Hardware:
 * - Arduino Uno
 * - L293D motor shield compatible with AFMotor library
 * - HC-SR04 ultrasonic sensor
 * - Servo motor
 * - Bluetooth module (e.g., HC-05), if using Bluetooth control
 *
 * Ultrasonic sensor:
 *   Echo -> A0
 *   Trig -> A1
 *
 * Servo signal -> Digital pin 10
 *
 * IMPORTANT:
 * The motor shield and servo pin assignments must be compatible
 * with your actual hardware.
 */

#include <Servo.h>
#include <AFMotor.h>

#define ECHO_PIN A0
#define TRIG_PIN A1
#define SERVO_PIN 10
#define MOTOR_SPEED 170
#define SERVO_CENTER 103

// Select ONE operating mode:
// 1 = Obstacle avoidance
// 2 = Bluetooth control
// 3 = Voice control (serial commands)
#define CONTROL_MODE 1

#define OBSTACLE_DISTANCE_CM 12
#define ULTRASONIC_TIMEOUT_US 30000UL

char value = '\0';
int distance = 0;

Servo servo;

AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

void forward();
void backward();
void left();
void right();
void Stop();

int ultrasonic();
int leftsee();
int rightsee();

void Obstacle();
void Bluetoothcontrol();
void voicecontrol();

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  servo.attach(SERVO_PIN);
  servo.write(SERVO_CENTER);
  delay(500);

  M1.setSpeed(MOTOR_SPEED);
  M2.setSpeed(MOTOR_SPEED);
  M3.setSpeed(MOTOR_SPEED);
  M4.setSpeed(MOTOR_SPEED);

  Stop();

  Serial.println("Robot car initialized.");
}

void loop() {
#if CONTROL_MODE == 1
  Obstacle();
#elif CONTROL_MODE == 2
  Bluetoothcontrol();
#elif CONTROL_MODE == 3
  voicecontrol();
#else
  Stop();
#endif
}

// Bluetooth commands:
// F = forward, B = backward, L = left,
// R = right, S = stop
void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();

    switch (value) {
      case 'F':
        forward();
        break;
      case 'B':
        backward();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'S':
        Stop();
        break;
      default:
        break;
    }
  }
}

// Obstacle avoidance
void Obstacle() {
  distance = ultrasonic();

  if (distance > 0 && distance <= OBSTACLE_DISTANCE_CM) {
    Stop();
    delay(150);

    backward();
    delay(300);
    Stop();
    delay(150);

    int leftDistance = leftsee();
    int rightDistance = rightsee();

    servo.write(SERVO_CENTER);
    delay(200);

    if (leftDistance == 0 && rightDistance == 0) {
      backward();
      delay(300);
    } else if (leftDistance > rightDistance) {
      left();
      delay(450);
    } else {
      right();
      delay(450);
    }

    Stop();
    delay(100);
  } else if (distance == 0) {
    // No valid echo: stop rather than assume the path is clear.
    Stop();
  } else {
    forward();
  }
}

// Voice-control commands received over Serial.
// ^ = forward, - = backward, < = left,
// > = right, * = stop
void voicecontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();

    switch (value) {
      case '^':
        forward();
        break;
      case '-':
        backward();
        break;
      case '<':
        left();
        delay(400);
        Stop();
        break;
      case '>':
        right();
        delay(400);
        Stop();
        break;
      case '*':
        Stop();
        break;
      default:
        break;
    }
  }
}

// Returns distance in centimeters.
// Returns 0 if no valid echo is received.
int ultrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ULTRASONIC_TIMEOUT_US);

  if (duration == 0) {
    return 0;
  }

  return (int)(duration / 58UL);
}

// Motor movement functions
void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}

// Measure distance to the left
int leftsee() {
  servo.write(180);
  delay(400);

  int d = ultrasonic();

  return d;
}

// Measure distance to the right
int rightsee() {
  servo.write(20);
  delay(400);

  int d = ultrasonic();

  return d;
}
