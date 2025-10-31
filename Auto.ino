/*
 * ================================================================
 *  Autonomous Car with Ultrasonic Sensors (Arduino UNO Version)
 * ================================================================
 *  Author: Robert Deines
 *  Description:
 *    This project implements a simple autonomous driving robot using
 *    four DC motors (controlled via H-bridges) and three ultrasonic
 *    sensors (front, left, right) for obstacle detection.
 * 
 *  Features:
 *    - Forward, backward, left, and right motion
 *    - Ultrasonic obstacle avoidance
 *    - PWM speed control on front motors
 *    - Simple braking and idle modes
 * 
 *  Hardware:
 *    - Microcontroller: Arduino UNO
 *    - Motor driver: L298N or similar dual H-bridge
 *    - Ultrasonic sensors: HC-SR04 (3x)
 * 
 *  Pinout summary:
 *    - Motor pins: defined below per motor
 *    - Ultrasonic: trigger/echo per direction
 * 
 *  GitHub Repository: https://github.com/10robert03/AutonomousCar_Arduino
 * ================================================================
 */


// ---------------------------- MOTOR PINS ----------------------------

// Left Front Motor
#define IN1 3   // Direction pin 1 (PWM capable)
#define IN2 5   // Direction pin 2

// Left Back Motor
#define IN1BACK 10
#define IN2BACK 11

// Right Front Motor
#define IN3 6   // Direction pin 1 (PWM capable)
#define IN4 9   // Direction pin 2

// Right Back Motor
#define IN3BACK 12
#define IN4BACK 13


// ---------------------------- ULTRASONIC PINS ----------------------------

#define SENDULTRASONICFRONT 2
#define SENDULTRASONICRIGHT 4
#define SENDULTRASONICLEFT 7

#define RECEIVEULTRASONICFRONT A0
#define RECEIVEULTRASONICRIGHT A1
#define RECEIVEULTRASONICLEFT  A2


// ---------------------------- FUNCTION DECLARATIONS ----------------------------

void forward(int speed);
void backward();
void turnLeft();
void turnRight();
void brake();
bool isObjectFront();
bool isObjectRight();
bool isObjectLeft();
void noDriving();


// ---------------------------- SETUP ----------------------------

void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN1BACK, OUTPUT);
  pinMode(IN2BACK, OUTPUT);
  pinMode(IN3BACK, OUTPUT);
  pinMode(IN4BACK, OUTPUT);

  // Ultrasonic trigger pins
  pinMode(SENDULTRASONICFRONT, OUTPUT);
  pinMode(SENDULTRASONICLEFT, OUTPUT);
  pinMode(SENDULTRASONICRIGHT, OUTPUT);

  // Ultrasonic echo pins
  pinMode(RECEIVEULTRASONICFRONT, INPUT);
  pinMode(RECEIVEULTRASONICRIGHT, INPUT);
  pinMode(RECEIVEULTRASONICLEFT, INPUT);

  // Start idle
  noDriving();
  delay(3000);
}


// ---------------------------- MAIN LOOP ----------------------------

void loop() {
  if (!isObjectFront()) {
    forward(80);  // Move forward at 80/255 PWM
  } 
  else {
    brake();
    noDriving();

    if (isObjectRight() && !isObjectLeft()) {
      turnLeft();
    } 
    else if (!isObjectRight() && isObjectLeft()) {
      turnRight();
    } 
    else {
      // Move backward while both sides are blocked
      while (isObjectRight() && isObjectLeft()) {
        backward();
      }

      brake();
      noDriving();

      // Reorient toward the open side
      if (!isObjectLeft()) turnLeft();
      else if (!isObjectRight()) turnRight();
    }
  }
}


// ---------------------------- MOTION CONTROL ----------------------------

void forward(int speed) {
  // Speed: 0–255 (PWM)
  analogWrite(IN1, speed);
  digitalWrite(IN2, LOW);
  digitalWrite(IN1BACK, HIGH);
  digitalWrite(IN2BACK, LOW);

  digitalWrite(IN3BACK, HIGH);
  digitalWrite(IN4BACK, LOW);
  analogWrite(IN3, speed);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN1BACK, LOW);
  digitalWrite(IN2BACK, HIGH);

  digitalWrite(IN3BACK, LOW);
  digitalWrite(IN4BACK, HIGH);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN1BACK, HIGH);
  digitalWrite(IN2BACK, LOW);

  digitalWrite(IN3BACK, LOW);
  digitalWrite(IN4BACK, HIGH);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);

  delay(300);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1BACK, LOW);
  digitalWrite(IN2BACK, HIGH);

  digitalWrite(IN4BACK, LOW);
  digitalWrite(IN3BACK, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);

  delay(300);
}

void brake() {
  // Active braking (short-circuit braking)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1BACK, HIGH);
  digitalWrite(IN2BACK, HIGH);

  digitalWrite(IN4BACK, HIGH);
  digitalWrite(IN3BACK, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);

  delay(200);
}

void noDriving() {
  // Disable all motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN1BACK, LOW);
  digitalWrite(IN2BACK, LOW);

  digitalWrite(IN3BACK, LOW);
  digitalWrite(IN4BACK, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// ---------------------------- ULTRASONIC MEASUREMENT ----------------------------

bool isObjectFront() {
  // Trigger pulse
  digitalWrite(SENDULTRASONICFRONT, LOW);
  delayMicroseconds(2);
  digitalWrite(SENDULTRASONICFRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENDULTRASONICFRONT, LOW);

  // Echo duration
  long duration = pulseIn(RECEIVEULTRASONICFRONT, HIGH);
  if (duration == 0) return false;

  // Distance in cm
  float distance = duration * 0.034 / 2;
  return (distance < 50);
}

bool isObjectLeft() {
  digitalWrite(SENDULTRASONICLEFT, LOW);
  delayMicroseconds(2);
  digitalWrite(SENDULTRASONICLEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENDULTRASONICLEFT, LOW);

  long duration = pulseIn(RECEIVEULTRASONICLEFT, HIGH);
  if (duration == 0) return false;

  float distance = duration * 0.034 / 2;
  return (distance < 10);
}

bool isObjectRight() {
  digitalWrite(SENDULTRASONICRIGHT, LOW);
  delayMicroseconds(2);
  digitalWrite(SENDULTRASONICRIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENDULTRASONICRIGHT, LOW);

  long duration = pulseIn(RECEIVEULTRASONICRIGHT, HIGH);
  if (duration == 0) return false;

  float distance = duration * 0.034 / 2;
  return (distance < 10);
}
