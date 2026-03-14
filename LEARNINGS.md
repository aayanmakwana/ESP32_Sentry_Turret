# Project Learnings
## ESP32 Automated Sentry Turret — Science Fair 2026

---

## 1. What I Built

An automated sentry turret using an ESP32 microcontroller that:
- Detects motion with a PIR sensor
- Scans for targets with an ultrasonic sensor on a servo
- Aims at the closest target using a pan/tilt bracket
- Fires a Nerf dart by mechanically pulling the trigger with a servo

---

## 2. Science Concepts Learned

### Passive Infrared (PIR) Sensing
PIR sensors detect changes in infrared radiation (heat). Every warm object — including humans — emits infrared light. When something warm moves across the sensor's field of view, it detects the change and triggers. This is the same technology used in security lights and alarm systems.

### Ultrasonic Distance Measurement
The HC-SR04 sensor works like a bat using echolocation. It sends a short burst of ultrasonic sound (40 kHz — too high for humans to hear), then measures how long it takes for the echo to return. Using the formula:

```
distance = (duration × speed of sound) / 2
```

The speed of sound in air is about 343 m/s (0.034 cm/µs). Dividing by 2 accounts for the sound travelling to the object and back.

### Servo Motor Control
Servo motors use PWM (Pulse Width Modulation) signals to move to specific angles. A pulse sent every 20ms controls position — a 1ms pulse = 0°, a 2ms pulse = 180°. The ESP32 uses its built-in LED control (LEDC) hardware to generate precise PWM signals.

### State Machines
Real systems are designed as state machines — the system is always in one defined state, and transitions only happen when specific conditions are met. My turret has 5 states:

```
IDLE → SCANNING → TARGETING → FIRING → COOLDOWN → IDLE
```

This prevents the system from doing two things at once and makes the behaviour predictable and safe. State machines are used in everything from traffic lights to spacecraft.

---

## 3. Engineering Challenges and How I Solved Them

### Challenge 1: Flash Write-Protection (Boya Chip Bug)
**Problem:** Every time I unplugged the ESP32, the flash memory chip would lock itself with write-protection. Uploading new code would fail with an MD5 mismatch error.

**Root Cause:** The ESP32-WROVER on this board uses a Boya flash chip. Unlike other brands, Boya chips re-enable write-protection bits in the status register on every power cycle. The CMP bit (bit 6 of SR2) was being set to 1, enabling full-chip write protection.

**Solution:** Used `esptool.py` to manually clear the write-protection bits (set status register to `0x0200`) before every upload session. Created a shell script (`fix_flash_wp.sh`) to automate this fix.

**Lesson:** Hardware bugs are real and sometimes you have to understand the chip's datasheet to fix them. Not all problems are code problems.

---

### Challenge 2: Servo Pin Conflicts
**Problem:** After testing with multiple test sketches, the pin numbers got out of sync. The main sketch had the pan servo on GPIO 14, but the physical wire was on GPIO 13. The trigger servo was on GPIO 15 in code but GPIO 4 in reality. The turret wasn't moving correctly.

**Solution:** Tested each servo individually using isolated test sketches to confirm which physical GPIO pin each servo was connected to. Then updated all sketches to match the confirmed physical wiring.

**Lesson:** Always test hardware components individually before combining them. Keep pin assignments consistent across all files. Comments in code can lie — the actual `#define` is the truth.

---

### Challenge 3: Servo Power — Common Ground
**Problem:** The MG996R servos (pan and tilt) need more current than the ESP32's 3.3V/5V pins can provide. When powered from the ESP32, the servos were weak and jittery, causing the ESP32 to reset.

**Solution:** Powered the MG996R servos from a separate 4×AA battery pack. However, the ESP32 and battery pack must share a common ground wire — without this, the PWM signal from the ESP32 has no reference point and the servo doesn't respond.

**Lesson:** In electronics, ground is not optional. When mixing power sources, always connect the grounds together.

---

### Challenge 4: GPIO 4 Conflict (RED_LED vs Trigger Servo)
**Problem:** An old test sketch had `#define RED_LED 4`. The trigger servo was later confirmed to be on GPIO 4. If both were active at the same time, the servo would receive garbage signals from the LED code.

**Solution:** Removed the RED_LED definition and all associated code from the test sketch. The final hardware design only has a green LED (GPIO 2).

**Lesson:** When reusing code across sketches, always audit for pin conflicts. One GPIO pin can only do one job at a time.

---

### Challenge 5: Trigger Servo Mounting
**Problem:** The SG90 trigger servo was glued to the Nerf gun with hot glue. During testing, the glue gave way and the servo detached.

**Solution:** Re-glued with more hot glue, applying it to a larger surface area and letting it cure fully before testing again.

**Lesson:** Physical mounting is just as important as the electronics. A well-written program can't compensate for a loose component.

---

## 4. What I Would Do Differently

1. **Label every wire** with masking tape and a marker from the beginning — tracing wires later wastes a lot of time.
2. **Document pin assignments in one place** (a single table) and update it every time something changes, rather than having pins scattered across multiple sketches.
3. **Test the physical mount before gluing** — use zip ties or clamps first to make sure the position is correct, then make it permanent.
4. **Use a breadboard first** before soldering or permanent connections — makes it much easier to change things.

---

## 5. Future Improvements

- Add a **camera with computer vision** (OpenCV) to identify humans vs. objects, so the turret only fires at real targets
- Add a **servo-controlled reload mechanism** so it can fire multiple darts automatically
- Replace the **ultrasonic sensor with LIDAR** for faster, more accurate distance measurement
- Add a **web interface** using the ESP32's built-in WiFi so the turret can be controlled from a phone
- Use a **3D printed housing** for a cleaner, sturdier build

---

## 6. Key Takeaways

> "Debugging hardware taught me more than writing the code."

- Real engineering involves solving problems that are not in any textbook
- Hardware and software must work together — you cannot fix a wiring problem with code
- Testing in small steps (one component at a time) saves hours of frustration
- Writing documentation as you go makes it much easier to pick up where you left off
