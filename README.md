# AutonomousCar_Arduino

An **autonomous car robot** built with an **Arduino UNO**, four DC motors (via L298N H-bridges), and **three ultrasonic sensors** (front, left, right).  
It drives forward until an obstacle is detected and then intelligently decides whether to turn left, right, or move backward before continuing.

---

## Features

- PWM-based **motor speed control** for smooth acceleration  
- **Obstacle detection** using three HC-SR04 ultrasonic sensors  
- **Autonomous navigation logic** (front/left/right decision tree)  
- **Electrical braking** and idle power-off modes  
- Compact and easy-to-reproduce **Arduino UNO-based** design

---

## Hardware Setup

| Component | Quantity | Notes |
|------------|-----------|-------|
| Arduino UNO | 1 | Main controller |
| L298N Dual H-Bridge | 2 | One per motor side (left/right) |
| DC Motors | 4 | Two front, two rear |
| HC-SR04 Ultrasonic Sensors | 3 | Front, Left, Right |
| Power Supply | 9–12 V | Depending on motors |

---

## Logic Overview

The car constantly checks for obstacles in front, left, and right.  
If there’s no obstacle → it moves forward.  
If an obstacle appears:
1. It stops (`brake()` + `noDriving()`).
2. Checks left/right distance.
3. Turns toward the clearer direction.
4. If both sides are blocked → drives backward until a side is free, then turns.

---

## Pin Configuration

### Motor Connections

| Motor | IN1 | IN2 | IN1BACK | IN2BACK |
|--------|-----|-----|----------|----------|
| Left Side | D3 | D5 | D10 | D11 |
| Right Side | D6 | D9 | D12 | D13 |

**PWM Pins Used:** D3, D6 → Front motors controlled with `analogWrite()`

---

### Ultrasonic Sensors

| Sensor | Trigger | Echo |
|---------|----------|------|
| Front | D2 | A0 |
| Right | D4 | A1 |
| Left | D7 | A2 |

---

## Function Overview

| Function | Description |
|-----------|--------------|
| `forward(int speed)` | Moves car forward at PWM speed (0–255). |
| `backward()` | Moves car backward at full power. |
| `turnLeft()` / `turnRight()` | Rotates car for 300 ms to turn. |
| `brake()` | Electrically brakes all motors for 200 ms. |
| `noDriving()` | Disables all motor outputs. |
| `isObjectFront()` / `isObjectLeft()` / `isObjectRight()` | Measures ultrasonic distance and returns `true` if obstacle detected. |

---

## Distance Calculation

The ultrasonic sensors measure distance using:
