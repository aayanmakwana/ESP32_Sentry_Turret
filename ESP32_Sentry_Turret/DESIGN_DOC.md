# Project: ESP32 Automated Sentry Turret

## Science Fair 2026 — Grade 8

**Project Name:** ESP32 Automated Sentry Turret
**Team:** [Student Name(s)]
**Date Started:** February 16, 2026
**Science Fair Date:** March 5, 2026
**Project Completion Deadline:** March 1, 2026
**Platform:** ESP32-WROVER-Dev + Arduino IDE 2.7.3

---

## 1. PROJECT OVERVIEW

### What Are We Building?

An automated sentry turret system that:
1. **Detects** movement using a PIR (Passive Infrared) motion sensor
2. **Scans** the area using an ultrasonic distance sensor mounted on a servo
3. **Locates** the target by measuring distance at each angle
4. **Aims** at the target using a pan/tilt servo mechanism
5. **Fires** a Nerf dart by mechanically pulling the trigger with a servo
6. **Alerts** with a buzzer and LED indicators

This mimics how real defense systems work (detect → track → engage) but in a safe, educational, and fun way using foam darts.

### How It Works — Step by Step

```
IDLE MODE (Scanning)
    │
    ▼
PIR Motion Sensor detects movement
    │
    ▼
System enters ALERT MODE
    │
    ▼
Ultrasonic sensor sweeps left-right on pan servo
    │
    ▼
Object detected within range (< 150 cm)?
    │
    ├── NO → Continue scanning → Return to IDLE after timeout
    │
    └── YES → Record angle and distance
                │
                ▼
            Pan/Tilt servos AIM at target
                │
                ▼
            Buzzer sounds WARNING beep
                │
                ▼
            Trigger servo FIRES Nerf dart
                │
                ▼
            Return to SCANNING mode
```

### System Block Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    ESP32-WROVER-Dev                       │
│                   (Brain / Controller)                    │
│                                                           │
│   INPUTS                              OUTPUTS             │
│   ──────                              ───────             │
│   PIR Sensor (GPIO 27) ────┐    ┌──── Pan Servo (GPIO 13) │
│   Ultrasonic TRIG (GPIO 5)─┤    ├──── Tilt Servo (GPIO 12)│
│   Ultrasonic ECHO (GPIO 18)┤    ├──── Trigger Servo(GPIO14)│
│                             │    ├──── Buzzer (GPIO 26)    │
│                             │    ├──── Red LED (GPIO 4)    │
│                             │    └──── Green LED (GPIO 2)  │
└─────────────────────────────────────────────────────────┘
                              │
                    ┌─────────┴─────────┐
                    │  External 6V      │
                    │  Battery Pack     │
                    │  (4x AA for       │
                    │   servos)         │
                    └───────────────────┘
```

---

## 2. WHAT WE WILL LEARN

### Science Concepts
| Concept | How It Applies |
|---|---|
| **Speed of Sound** | Ultrasonic sensor measures distance using time-of-flight of sound waves (343 m/s). Distance = (time × speed) / 2 |
| **Infrared Radiation** | PIR sensor detects body heat (infrared radiation) emitted by warm objects |
| **Trigonometry** | Converting servo angle + distance into X/Y target position using sin/cos |
| **Projectile Motion** | Understanding how the Nerf dart travels (arc, gravity, air resistance) |
| **Reflection & Echo** | Ultrasonic waves bounce off objects and return to the sensor |

### Engineering Concepts
| Concept | How It Applies |
|---|---|
| **Feedback Control Systems** | Sensor reads → processor decides → actuator acts → sensor reads again (control loop) |
| **Pulse Width Modulation (PWM)** | Servos are controlled by PWM signals — pulse width determines angle |
| **Digital vs Analog Signals** | PIR outputs digital (HIGH/LOW), ultrasonic uses timed pulses |
| **Power Management** | Servos need more current than the ESP32 can provide — separate power supply needed |
| **Systems Integration** | Making sensors, actuators, and code all work together as one system |

### Programming Concepts
| Concept | How It Applies |
|---|---|
| **State Machines** | System has states: IDLE, SCANNING, TARGETING, FIRING |
| **Functions** | Breaking code into reusable pieces (readDistance(), aimAt(), fire()) |
| **Variables & Data Types** | Storing angle, distance, target position |
| **Control Flow** | if/else decisions, for loops for scanning |
| **Serial Debugging** | Using Serial Monitor to see what the system is "thinking" |

---

## 3. PARTS INVENTORY

### Parts We Already Have (from Freenove FNK0047 Kit)

| # | Component | Qty | Purpose in Project |
|---|---|---|---|
| 1 | ESP32-WROVER-Dev Board | 1 | Main controller (the brain) |
| 2 | HC-SR501 PIR Motion Sensor | 1 | Detects human/animal movement via body heat |
| 3 | HC-SR04 Ultrasonic Sensor | 1 | Measures distance to target (2cm–400cm range) |
| 4 | SG90 Micro Servo | 1 | Pulls the Nerf gun trigger |
| 5 | Active Buzzer | 1 | Warning/firing sound alert |
| 6 | LEDs (Red, Green) | 2+ | Status indicators (armed, target detected) |
| 7 | 220 ohm Resistors | 2 | Current limiting for LEDs |
| 8 | Breadboard | 1 | Prototyping circuit connections |
| 9 | Jumper Wires (M-M, M-F, F-F) | 3 sets | Connecting everything together |
| 10 | USB Cable | 1 | Programming and serial monitor |
| 11 | B10K Potentiometer | 1 | Optional: manual sensitivity dial |
| 12 | GY-521 Accelerometer (MPU-6050) | 1 | Optional: detect if turret is bumped/moved |

### Parts We Need to Buy

| # | Component | Qty | Purpose | Link | Est. Cost |
|---|---|---|---|---|---|
| 1 | **MG996R Servo Motors** | 2 | Pan (horizontal) and Tilt (vertical) aiming | [Amazon - MG996R 2-Pack](https://www.amazon.com/dp/B0BYNBZHD1) | ~$12 |
| 2 | **Pan/Tilt Servo Bracket Kit** (for MG996R) | 1 | Metal frame to hold servos as a turret | [Amazon - elechawk Pan Tilt Bracket](https://www.amazon.com/Servo-Mount-Bracket-MG996R-Steering/dp/B07PQ12TXS) | ~$10 |
| 3 | **Nerf Jolt Blaster** (or similar small single-shot) | 1 | The "weapon" — fires foam darts | Search "Nerf Jolt" at local store or Amazon | ~$8 |
| 4 | **4x AA Battery Holder with leads** | 1 | 6V power for MG996R servos (too powerful for USB) | Search "4xAA battery holder with switch" on Amazon | ~$5 |
| 5 | **AA Batteries** | 4 | Power supply for servos | Any store | ~$5 |
| 6 | **Hot Glue Gun + Glue Sticks** | 1 | Mount Nerf gun to bracket, secure wires | Any store or Amazon | ~$8 |
| 7 | **Zip Ties (small assortment)** | 1 pack | Cable management, securing components | Any store | ~$3 |
| 8 | **Small wood board or thick cardboard** (8"x10") | 1 | Base plate to mount the entire turret | Hardware store or around the house | ~$3 |
| | | | | **TOTAL** | **~$54** |

**ORDER PARTS BY: February 17, 2026 (Monday) — Select Amazon Prime for delivery by Feb 19**

---

## 4. DETAILED DESIGN

### 4.1 Pin Assignment Table

| GPIO Pin | Component | Direction | Notes |
|---|---|---|---|
| GPIO 13 | Pan Servo (MG996R) | OUTPUT | Horizontal rotation (0-180°) |
| GPIO 12 | Tilt Servo (MG996R) | OUTPUT | Vertical angle (60-120°) |
| GPIO 14 | Trigger Servo (SG90) | OUTPUT | Pulls Nerf trigger |
| GPIO 5 | Ultrasonic TRIG | OUTPUT | Sends ultrasonic pulse |
| GPIO 18 | Ultrasonic ECHO | INPUT | Receives reflected pulse |
| GPIO 27 | PIR Motion Sensor | INPUT | HIGH when motion detected |
| GPIO 26 | Active Buzzer | OUTPUT | Alert sounds |
| GPIO 2 | Green LED | OUTPUT | System armed / scanning |
| GPIO 4 | Red LED | OUTPUT | Target detected / firing |

### 4.2 Wiring Diagram (Text)

```
ESP32-WROVER-Dev
┌────────────────────┐
│                    │
│ GPIO 13 ──────────────── Pan Servo (MG996R) Signal [ORANGE wire]
│ GPIO 12 ──────────────── Tilt Servo (MG996R) Signal [ORANGE wire]
│ GPIO 14 ──────────────── Trigger Servo (SG90) Signal [ORANGE wire]
│                    │
│ GPIO 5  ──────────────── HC-SR04 TRIG
│ GPIO 18 ──────────────── HC-SR04 ECHO
│                    │
│ GPIO 27 ──────────────── HC-SR501 PIR OUT
│                    │
│ GPIO 26 ──────────────── Buzzer (+)
│ GPIO 2  ───[220Ω]────── Green LED (+)
│ GPIO 4  ───[220Ω]────── Red LED (+)
│                    │
│ GND ──────────────────── Common GND (all components)
│ 3.3V ─────────────────── PIR VCC, HC-SR04 VCC
│                    │
└────────────────────┘

Battery Pack (4x AA = 6V)
┌────────────────────┐
│ (+) ──────────────────── MG996R Servo 1 VCC [RED wire]
│                    ├──── MG996R Servo 2 VCC [RED wire]
│                    ├──── SG90 Servo VCC [RED wire]
│ (-) ──────────────────── All Servo GND [BROWN wire]
│                    └──── ESP32 GND (common ground!)
└────────────────────┘

*** IMPORTANT: Battery GND must connect to ESP32 GND (common ground) ***
*** IMPORTANT: Do NOT power MG996R servos from ESP32 5V pin — not enough current ***
```

### 4.3 Servo Positions Reference

**Pan Servo (Horizontal):**
```
     90° (center/forward)
      │
 15° ─┼─ 165°
(left)  (right)

Scan range: 15° to 165° (matching radar sketch)
```

**Tilt Servo (Vertical):**
```
 60° (tilted up)
  │
 90° (level/straight)
  │
 120° (tilted down)

Default: 90° (level)
Adjust based on Nerf gun mounting angle
```

**Trigger Servo:**
```
 0°  = Rest (not pulling trigger)
 90° = Pull (trigger fully pressed)

Action: Snap from 0° to 90°, hold 300ms, return to 0°
```

### 4.4 Software Design — State Machine

```
┌──────────┐
│   IDLE   │ ← Green LED on, slow scan
│          │   PIR not triggered
└────┬─────┘
     │ PIR goes HIGH
     ▼
┌──────────┐
│ SCANNING │ ← Green LED blinking
│          │   Ultrasonic sweeps 15°-165°
│          │   Looking for object < 150cm
└────┬─────┘
     │ Object found at angle X, distance D
     ▼
┌──────────┐
│ TARGETING│ ← Red LED on
│          │   Pan servo moves to angle X
│          │   Tilt servo adjusts for distance
│          │   Buzzer: short beeps
└────┬─────┘
     │ Aimed and ready
     ▼
┌──────────┐
│ FIRING   │ ← Red LED blinking
│          │   Buzzer: long tone
│          │   Trigger servo pulls (0° → 90°)
│          │   Wait 500ms
│          │   Trigger servo releases (90° → 0°)
└────┬─────┘
     │ Done firing
     ▼
┌──────────┐
│ COOLDOWN │ ← 3 second pause
│          │   Both LEDs off
│          │   Reload Nerf dart manually
└────┬─────┘
     │ Timeout
     ▼
   IDLE (restart)
```

### 4.5 Code Structure (Pseudo-code)

```cpp
// === LIBRARIES ===
#include <ESP32Servo.h>

// === PIN DEFINITIONS ===
#define PAN_SERVO_PIN    13
#define TILT_SERVO_PIN   12
#define TRIGGER_SERVO_PIN 14
#define TRIG_PIN          5
#define ECHO_PIN         18
#define PIR_PIN          27
#define BUZZER_PIN       26
#define GREEN_LED_PIN     2
#define RED_LED_PIN       4

// === STATE MACHINE ===
enum State { IDLE, SCANNING, TARGETING, FIRING, COOLDOWN };
State currentState = IDLE;

// === OBJECTS ===
Servo panServo;
Servo tiltServo;
Servo triggerServo;

// === KEY FUNCTIONS ===

long readDistance()
    // Send ultrasonic pulse, measure echo time
    // Return distance in cm

int scanForTarget()
    // Sweep pan servo from 15° to 165°
    // At each angle, read distance
    // If object found < 150cm, return that angle
    // If no object found, return -1

void aimAt(int angle, long distance)
    // Move pan servo to target angle
    // Adjust tilt based on distance (closer = aim slightly down)

void fire()
    // Sound buzzer
    // Move trigger servo from 0° to 90° (pull trigger)
    // Wait 300ms
    // Move trigger servo back to 0° (release)

// === MAIN LOOP ===
void loop()
    switch(currentState):
        case IDLE:
            Green LED on
            If PIR detects motion → currentState = SCANNING

        case SCANNING:
            targetAngle = scanForTarget()
            If target found → currentState = TARGETING
            If timeout (no target) → currentState = IDLE

        case TARGETING:
            Red LED on
            aimAt(targetAngle, targetDistance)
            Short buzzer beeps
            currentState = FIRING

        case FIRING:
            fire()
            currentState = COOLDOWN

        case COOLDOWN:
            Wait 3 seconds
            currentState = IDLE
```

### 4.6 Physical Assembly Overview

```
         SIDE VIEW                          TOP VIEW
         ─────────                          ────────

     ┌─── Nerf Gun ───┐              ┌──── Nerf Gun ────┐
     │   ══════════>   │  ← barrel   │    ══════════>    │
     └──────┬──────────┘              └────────┬─────────┘
            │ (hot glued)                      │
     ┌──────┴──────┐                  ┌────────┴─────────┐
     │  Tilt Servo │                  │    Tilt Servo     │
     │  (up/down)  │                  │                   │
     └──────┬──────┘                  └────────┬─────────┘
            │                                  │
     ┌──────┴──────┐                  ┌────────┴─────────┐
     │  Pan Servo  │                  │    Pan Servo      │
     │  (left/right)│                 │   (rotates base)  │
     └──────┬──────┘                  └────────┬─────────┘
            │                                  │
    ════════╧══════════           ═════════════╧═══════════
        Base Board                       Base Board
    (wood/cardboard)              ┌─PIR    ┌─Ultrasonic
                                  │        │
                                  └────────┘
                                  (front-mounted sensors)

Sensor Placement:
- Ultrasonic sensor: mounted on the pan servo horn (rotates with turret)
- PIR sensor: fixed on the base, pointing forward (wide detection angle)
- SG90 trigger servo: attached near the Nerf gun trigger with string/wire
```

---

## 5. WORKING BACKWARDS PLAN — DAY BY DAY

### Week Overview

| Week | Dates | Focus |
|---|---|---|
| Week 1 | Feb 16-22 | Order parts, test existing components, learn basics |
| Week 2 | Feb 23-28 | Build, code, integrate, test |
| Final | Mar 1-4 | Polish, prepare presentation, practice |

---

### DAY-BY-DAY SCHEDULE

#### Day 1 — Sunday, February 16 ✅ (TODAY)
**Focus: Project Setup & Parts Ordering**
- [x] Set up project workspace on computer
- [x] Review and identify all parts from Freenove kit
- [x] Read this design document completely
- [ ] Order all parts from Amazon (use Prime for fastest delivery)
- [ ] Watch 2-3 YouTube videos on "Arduino sentry turret" to understand the concept
- **Time: 1-2 hours**

#### Day 2 — Monday, February 17
**Focus: Test PIR Sensor**
- [ ] Wire PIR sensor (HC-SR501) to ESP32 on breadboard
- [ ] Write simple sketch: print "Motion detected!" to Serial Monitor when PIR goes HIGH
- [ ] Understand PIR sensitivity and delay adjustment pots (two orange knobs on back)
- [ ] Test detection range — walk toward it from different distances
- [ ] Record: at what distance does it detect you? What's the detection angle?
- **Goal: Understand how the PIR "sees" movement**
- **Time: 1 hour**

```
Simple PIR Test Wiring:
  PIR VCC → ESP32 3.3V
  PIR GND → ESP32 GND
  PIR OUT → ESP32 GPIO 27
```

#### Day 3 — Tuesday, February 18
**Focus: Test Ultrasonic Sensor**
- [ ] Wire HC-SR04 ultrasonic sensor to ESP32
- [ ] Write sketch that continuously prints distance in cm to Serial Monitor
- [ ] Test accuracy: measure with a ruler vs what the sensor reports
- [ ] Learn the sensor's limits: minimum distance (~2cm), maximum (~400cm), cone angle (~15°)
- [ ] Move your hand closer and farther — watch the numbers change
- **Goal: Understand how ultrasonic distance measurement works**
- **Time: 1 hour**

```
Simple Ultrasonic Test Wiring:
  HC-SR04 VCC  → ESP32 3.3V (or 5V)
  HC-SR04 GND  → ESP32 GND
  HC-SR04 TRIG → ESP32 GPIO 5
  HC-SR04 ECHO → ESP32 GPIO 18
```

#### Day 4 — Wednesday, February 19
**Focus: Test SG90 Servo + Receive Parts**
- [ ] Wire SG90 servo to ESP32
- [ ] Write sketch that sweeps servo from 0° to 180° and back
- [ ] Receive Amazon delivery — unbox and verify all parts
- [ ] Identify MG996R servo wires: BROWN=GND, RED=VCC, ORANGE=Signal
- [ ] Test one MG996R servo with battery pack (NOT from ESP32 power!)
- **Goal: Understand PWM servo control, verify new parts work**
- **Time: 1-2 hours**

```
MG996R Test Wiring:
  MG996R ORANGE (signal) → ESP32 GPIO 13
  MG996R RED (VCC)       → Battery Pack (+)
  MG996R BROWN (GND)     → Battery Pack (-)
  Battery Pack (-)       → ESP32 GND  *** IMPORTANT: Common ground! ***
```

#### Day 5 — Thursday, February 20
**Focus: Assemble Pan/Tilt Bracket**
- [ ] Follow the bracket kit instructions to assemble the pan/tilt mechanism
- [ ] Install both MG996R servos into the bracket
- [ ] Wire both servos to ESP32 (GPIO 13 = pan, GPIO 12 = tilt)
- [ ] Write sketch that moves pan left-right and tilt up-down
- [ ] Test: use Serial Monitor to type an angle, watch servo move to it
- **Goal: Two-axis aiming system working and responsive**
- **Time: 1-2 hours**

#### Day 6 — Friday, February 21
**Focus: Mount Ultrasonic Sensor + Scanning Code**
- [ ] Mount/attach ultrasonic sensor to the pan servo horn (hot glue or zip tie)
- [ ] Now the ultrasonic sensor rotates WITH the turret as it pans
- [ ] Combine servo sweep + ultrasonic reading into a scanning function
- [ ] Print angle and distance to Serial Monitor as it scans
- [ ] Verify: place an object (book, box) in front — does it detect at the right angle?
- **Goal: Turret can scan and find objects by angle and distance**
- **Time: 1.5 hours**

#### Day 7 — Saturday, February 22
**Focus: Mount Nerf Gun + Trigger Mechanism**
- [ ] Hot glue the Nerf gun onto the tilt servo bracket (barrel pointing forward)
- [ ] Attach SG90 servo near the Nerf trigger
- [ ] Connect SG90 to trigger using a short piece of stiff wire or strong string
- [ ] Test: when SG90 moves from 0° to 90°, does it pull the trigger?
- [ ] Adjust servo angle and string length until trigger fires reliably
- [ ] Load a dart and test fire!
- **Goal: Nerf gun fires when the trigger servo activates**
- **Time: 2 hours (mechanical work — be patient, this is the trickiest part)**

**Tips for trigger mechanism:**
```
  String method:
  - Tie fishing line or string to SG90 servo horn
  - Loop it around/through the Nerf trigger
  - When servo rotates, string pulls trigger

  Direct push method:
  - Attach a stiff wire (paperclip) to SG90 servo horn
  - Position SG90 so the wire pushes the trigger directly
  - More reliable but needs precise positioning
```

#### Day 8 — Sunday, February 23
**Focus: Write the Full Code — Part 1 (Sensors + Scanning)**
- [ ] Create new sketch: `ESP32_Sentry_Turret.ino`
- [ ] Implement state machine structure (IDLE, SCANNING, TARGETING, FIRING, COOLDOWN)
- [ ] Code the IDLE state: green LED on, waiting for PIR trigger
- [ ] Code the SCANNING state: sweep and find closest object
- [ ] Test with Serial Monitor — watch state transitions
- **Goal: System detects motion and scans for targets**
- **Time: 2 hours**

#### Day 9 — Monday, February 24
**Focus: Write the Full Code — Part 2 (Aiming + Firing)**
- [ ] Code the TARGETING state: pan/tilt servos aim at detected target
- [ ] Code the FIRING state: trigger servo fires
- [ ] Code the COOLDOWN state: pause, reset, return to IDLE
- [ ] Add buzzer alerts (beep pattern for each state)
- [ ] Add LED indicators (green = scanning, red = target/firing)
- [ ] Test the full cycle: detect → scan → aim → fire
- **Goal: Complete working system end-to-end**
- **Time: 2 hours**

#### Day 10 — Tuesday, February 25
**Focus: Testing & Tuning**
- [ ] Set up a target (stuffed animal, cardboard cutout) at 50cm, 100cm, 150cm
- [ ] Run the turret — does it detect, aim, and hit the target?
- [ ] Tune these values in code:
  - Scan speed (delay between steps)
  - Detection threshold distance
  - Tilt angle compensation for different distances
  - Trigger pull timing
- [ ] Fix any bugs — use Serial Monitor to debug
- [ ] Test 10 times in a row — record hit rate
- **Goal: System works reliably 7+ out of 10 times**
- **Time: 2 hours**

#### Day 11 — Wednesday, February 26
**Focus: Reliability & Edge Cases**
- [ ] Test with different targets (different sizes, distances)
- [ ] What happens if no target is found? (should return to IDLE cleanly)
- [ ] What happens if target moves during aiming? (should re-scan)
- [ ] Add a manual arm/disarm button (optional — use one of the kit push buttons)
- [ ] Clean up wiring — shorten wires, secure with zip ties
- [ ] Make sure nothing is loose or wobbly
- **Goal: Robust system that doesn't crash or get stuck**
- **Time: 1.5 hours**

#### Day 12 — Thursday, February 27
**Focus: Build Presentation Board**
- [ ] Create a tri-fold presentation board with:
  - Project title and name
  - "Question": Can we build an automated system that detects and responds to intruders?
  - "Hypothesis": Using sensors and microcontrollers, we can build a system that detects, tracks, and engages targets autonomously
  - "Materials": List all parts with photos
  - "Procedure": Step-by-step build process
  - "How It Works": The system diagram from this document
  - "What I Learned": Science/engineering concepts (Section 2 of this doc)
  - "Results": Hit rate data from testing (e.g., "Hit target 8/10 times at 1 meter")
  - "Conclusion": What worked, what was hard, what I'd improve
- [ ] Print wiring diagram and photos of the build process
- **Goal: Professional-looking presentation board**
- **Time: 2-3 hours**

#### Day 13 — Friday, February 28
**Focus: Final Assembly & Demo Practice**
- [ ] Mount everything neatly on the base board
- [ ] Label components with small tags (optional but impressive)
- [ ] Practice the demo 3 times start to finish
- [ ] Practice explaining how each part works (2-3 minutes max)
- [ ] Prepare answers for likely judge questions:
  - "How does the ultrasonic sensor work?" → Sound waves, speed of sound, echo timing
  - "Why did you use an ESP32?" → Dual core, fast, WiFi/BT capable, enough GPIO pins
  - "What was the hardest part?" → (Your honest answer)
  - "What would you improve?" → Camera-based tracking, faster servos, multiple darts
- **Goal: Ready to present confidently**
- **Time: 2 hours**

#### Day 14 — Saturday, March 1 🎯 PROJECT COMPLETE
**Focus: Final Check & Backup**
- [ ] Full system test — 3 complete run-throughs
- [ ] Charge/replace batteries
- [ ] Pack extra batteries, darts, USB cable, laptop (for reprogramming if needed)
- [ ] Make sure everything fits in a box for transport
- [ ] Backup code to USB drive
- **Goal: Everything packed and ready for science fair**
- **Time: 1 hour**

#### March 2-4 — Buffer Days
- Handle any last-minute issues
- Extra practice time
- Rest before the fair!

#### March 5 — SCIENCE FAIR DAY 🏆
- Set up turret on table
- Quick test before judges arrive
- Demo and present with confidence!

---

## 6. DAILY TIME SUMMARY

| Day | Date | Task | Est. Time |
|---|---|---|---|
| 1 | Feb 16 (Sun) | Setup, order parts | 1-2 hrs |
| 2 | Feb 17 (Mon) | Test PIR sensor | 1 hr |
| 3 | Feb 18 (Tue) | Test ultrasonic sensor | 1 hr |
| 4 | Feb 19 (Wed) | Test servos, receive parts | 1-2 hrs |
| 5 | Feb 20 (Thu) | Assemble pan/tilt bracket | 1-2 hrs |
| 6 | Feb 21 (Fri) | Mount ultrasonic + scanning code | 1.5 hrs |
| 7 | Feb 22 (Sat) | Mount Nerf gun + trigger | 2 hrs |
| 8 | Feb 23 (Sun) | Code Part 1 — sensors + scanning | 2 hrs |
| 9 | Feb 24 (Mon) | Code Part 2 — aiming + firing | 2 hrs |
| 10 | Feb 25 (Tue) | Testing & tuning | 2 hrs |
| 11 | Feb 26 (Wed) | Reliability & cleanup | 1.5 hrs |
| 12 | Feb 27 (Thu) | Presentation board | 2-3 hrs |
| 13 | Feb 28 (Fri) | Final assembly & demo practice | 2 hrs |
| 14 | Mar 1 (Sat) | Final check & pack | 1 hr |
| | | **TOTAL** | **~20-24 hrs** |

---

## 7. TROUBLESHOOTING GUIDE

| Problem | Likely Cause | Fix |
|---|---|---|
| Servo jitters/shakes | Not enough power from USB | Use battery pack for servos |
| Ultrasonic reads 0 | Wiring wrong or ECHO/TRIG swapped | Double-check wiring, GPIO pins |
| PIR always HIGH | Sensitivity too high or needs warmup | Wait 30s after power-on; turn sensitivity pot down |
| PIR never triggers | Sensitivity too low or wired wrong | Turn sensitivity pot up; check VCC is 3.3V |
| Nerf doesn't fire | Trigger servo not pulling hard enough | Adjust string length; use stiffer wire; check servo angle range |
| ESP32 resets randomly | Servo drawing too much current from board | Separate power supply for servos! |
| Flash upload fails (MD5 error) | Boya flash chip write-protection (known issue) | Run `fix_flash_wp.sh` script (see project files) |
| Code compiles but nothing happens | Wrong board selected in Arduino IDE | Tools → Board → ESP32 Wrover Module |

---

## 8. SAFETY RULES

1. **NEVER aim at faces or eyes** — even Nerf darts can hurt at close range
2. **Always wear safety glasses** during testing
3. **Keep fingers away from servos** when powered — they can pinch
4. **Disconnect battery** when making wiring changes
5. **Adult supervision** when using hot glue gun
6. **At the science fair**: Keep turret disarmed until demo time, warn bystanders

---

## 9. FILES & RESOURCES

| File | Location | Description |
|---|---|---|
| Design Doc | `ESP32_Sentry_Turret/DESIGN_DOC.md` | This document |
| Radar Sketch | `ESP32_Radar/ESP32_Radar.ino` | Original radar project (reference) |
| Flash Fix Script | `fix_flash_wp.sh` | Fixes Boya flash write-protection |
| Project Log | `PROJECT_LOG.md` | Daily activity log |
| Part List Image | `freenove_part_list.jpg` | Kit contents reference |
| Component Photos | `images/` | Photos of our components |

### Useful Links
- [Freenove Kit Tutorial](https://docs.freenove.com/projects/fnk0047/en/latest/)
- [Freenove Kit GitHub (code + datasheets)](https://github.com/Freenove/Freenove_Ultimate_Starter_Kit_for_ESP32)
- [ESP32Servo Library](https://github.com/madhephaestus/ESP32Servo)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

---

*Document created: February 16, 2026*
*Last updated: February 16, 2026*
