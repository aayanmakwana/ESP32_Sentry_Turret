# Project Requirements
## ESP32 Automated Sentry Turret — Science Fair 2026

---

## 1. Project Goal

Build an automated turret that detects, tracks, and fires at a moving target using an ESP32 microcontroller and low-cost sensors — without human input after arming.

---

## 2. Hardware Requirements

| Component | Model | Purpose |
|---|---|---|
| Microcontroller | ESP32-WROVER-Dev | Main brain — runs all code |
| Pan Servo | MG996R | Rotates turret left/right |
| Tilt Servo | MG996R | Angles gun up/down |
| Trigger Servo | SG90 | Pulls Nerf gun trigger |
| Motion Sensor | HC-SR501 PIR | Detects body heat / movement |
| Distance Sensor | HC-SR04 Ultrasonic | Measures distance to target |
| Buzzer | Active buzzer | Audio alerts |
| Status LED | Green LED | Shows armed/scanning state |
| Power | 4×AA battery pack | Powers the two MG996R servos |
| Nerf Gun | Nerf Jolt | Fires foam darts safely |

### Pin Assignments

| Component | GPIO Pin |
|---|---|
| Pan Servo (MG996R) | GPIO 13 |
| Tilt Servo (MG996R) | GPIO 32 |
| Trigger Servo (SG90) | GPIO 4 |
| Ultrasonic TRIG | GPIO 5 |
| Ultrasonic ECHO | GPIO 18 |
| PIR Sensor | GPIO 27 |
| Buzzer | GPIO 26 |
| Green LED | GPIO 2 |

---

## 3. Functional Requirements

### Detection
- The system must detect human movement using the PIR sensor
- PIR sensor must complete a 10-second warmup on startup before arming
- Detection range: up to 150 cm

### Scanning
- On motion detection, the pan servo must sweep from 15° to 165°
- The ultrasonic sensor must measure distance at every 3° step
- The system must identify the angle of the closest object in range
- Scan must time out after 8 seconds if no target is found

### Targeting
- Pan servo must rotate to the detected target angle
- Tilt servo must adjust based on target distance:
  - Distance < 50 cm → tilt down
  - Distance 50–120 cm → level
  - Distance > 120 cm → tilt up
- System must verify the target is still present before firing

### Firing
- Three short warning beeps before firing
- Trigger servo moves from REST (180°) to PULL (0°) to fire
- Trigger held for 400 ms then released back to 180°

### Reset / Cooldown
- After firing, turret returns to center (pan 90°, tilt 90°)
- 3-second cooldown before re-arming
- System re-arms automatically after cooldown

### Manual Override (Serial Monitor)
- Must support manual commands: scan, fire, reset, distance check, tilt control, auto toggle

---

## 4. Software Requirements

- Language: C++ (Arduino framework)
- Library: ESP32Servo for PWM servo control
- Architecture: State machine (IDLE → SCANNING → TARGETING → FIRING → COOLDOWN)
- Serial baud rate: 115200
- Must not block the main loop during scanning (uses delay-based stepping)

---

## 5. Safety Requirements

- Only Nerf foam darts may be used — no projectiles that could cause injury
- Buzzer warning must sound before every firing sequence
- System must have a manual off switch (Serial Monitor `A` command or power off)
- Turret must return to center position on reset

---

## 6. Physical Requirements

- Turret must rotate at least 150° horizontally (15° to 165°)
- All components must be securely mounted (no loose wires during operation)
- Battery pack must be separate from ESP32 power to avoid servo voltage drop
- Common ground must be shared between battery pack and ESP32
