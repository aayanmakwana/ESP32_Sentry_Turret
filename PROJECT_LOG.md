# ESP32 Sentry Turret Project — Activity Log

**Project Name:** ESP32 Automated Sentry Turret (Science Fair 2026)
**Previous Project:** ESP32 Radar (completed, used as learning reference)
**Board:** ESP32-WROVER-Dev (ESP32-D0WDQ6 rev v1.0)
**IDE:** Arduino IDE 2.7.3
**ESP32 Board Package:** 3.3.7
**Workspace:** C:\Users\AVENGER\ESP32
**Repository:** Local Git (initialized)

---

## February 16, 2026

### 1. Project Setup

- Created project workspace at `C:\Users\AVENGER\ESP32`
- Initialized Git repository
- Created Arduino sketch folder: `ESP32_Radar/ESP32_Radar.ino`
- Added `claude-code` command alias for quick CLI access

### 2. Radar Sketch Created

Created the main radar sketch (`ESP32_Radar.ino`) with the following functionality:

**Hardware Components:**
| Component          | GPIO Pin |
|--------------------|----------|
| Servo Motor        | GPIO 13  |
| Ultrasonic TRIG    | GPIO 5   |
| Ultrasonic ECHO    | GPIO 18  |
| TFT Display        | Via SPI (configured in TFT_eSPI User_Setup.h) |

**Libraries Used:**
- `TFT_eSPI` — TFT display driver
- `ESP32Servo` — Servo motor control
- `SPI.h` — SPI communication
- `Arduino.h` — Core Arduino framework

**Sketch Features:**
- Ultrasonic distance measurement using HC-SR04 sensor
- Servo sweeps between 15° and 165° (back and forth)
- TFT display renders a radar-style UI with:
  - Four concentric range circles (25%, 50%, 75%, 100% of max radius)
  - Green sweep line following the servo angle
  - Red dots indicating detected objects within 200 cm
- Display center: (160, 220), radar radius: 150 pixels

**Sketch Size:**
- Program storage: 384,667 bytes (29% of 1,310,720 bytes)
- Dynamic memory: 22,952 bytes (7% of 327,680 bytes)

### 3. Flash Write-Protection Issue (Resolved)

**Problem:**
Upload to ESP32 failed with the error:
`A fatal error occurred: MD5 of file does not match data in flash!`

**Diagnosis:**
- Flash erase was completing in 0.0 seconds (abnormally fast, should take >1 second)
- Manual write test confirmed: data written to flash was not persisting
- Flash chip identified as **Boya (manufacturer ID: `d8`)**, 4MB, 3.3V
- Flash status register read `0x4200` — the **CMP bit (SR2 bit 6)** was set, enabling full-chip write protection

**Root Cause:**
The Boya flash chip on this board has a recurring issue where the write-protection bits in the status register get re-enabled. This is a known hardware-level quirk with Boya flash chips — they are more aggressive about latching write-protect compared to Winbond or GigaDevice flash chips. There is no permanent software fix; it will recur.

**Solution Applied:**
```
# Step 1: Read flash status (confirmed 0x4200 = write-protected)
esptool.exe --port COM4 read-flash-status --bytes 2

# Step 2: Clear write-protect bits, keep Quad Enable bit
esptool.exe --port COM4 write-flash-status --bytes 2 0x0200

# Step 3: Verify status is now 0x0200
esptool.exe --port COM4 read-flash-status --bytes 2

# Step 4: Erase flash (should now take >1 second)
esptool.exe --port COM4 erase-flash
```

**Result:**
After clearing the write-protect bits, flash erase took 1.4 seconds (correct) and the sketch uploaded successfully with all four regions verified:

| Region          | Address      | Size    | Status        |
|-----------------|-------------|---------|---------------|
| Bootloader      | 0x00001000  | 23 KB   | Hash verified |
| Partition table | 0x00008000  | 3 KB    | Hash verified |
| OTA data        | 0x0000e000  | 8 KB    | Hash verified |
| Sketch          | 0x00010000  | 385 KB  | Hash verified |

**Prevention:**
Created a quick-fix script at `C:\Users\AVENGER\ESP32\fix_flash_wp.sh` that can be run anytime this issue recurs:
```
bash ~/ESP32/fix_flash_wp.sh
```

### 4. Other Troubleshooting Steps Attempted (Before Root Cause Found)

These did NOT fix the issue but are documented for reference:
- Changed Flash Mode from QIO to DIO
- Lowered Flash Frequency from 80MHz to 40MHz
- Lowered Upload Speed from 921600 to 115200
- Enabled "Erase All Flash Before Sketch Upload" in Arduino IDE
- Ran `esptool erase-flash` from command line (erased in 0.0s — flash was write-protected)

---

## February 17, 2026

### 1. Science Fair Project Decided — ESP32 Automated Sentry Turret

Pivoted from the radar project to a full **Automated Sentry Turret** for the Grade 8 Science Fair (March 5, 2026).

**Concept:** An automated system that detects movement (PIR sensor), scans for targets (ultrasonic sensor on servo), aims (pan/tilt bracket), and fires a Nerf dart (trigger servo).

**Key decisions made:**
- Sensor-based detection (not camera-based) — using PIR + ultrasonic
- No 3D printer available — buying pre-made pan/tilt metal bracket online
- Nerf Jolt (single-shot) as the firing mechanism
- SG90 servo from kit will pull the Nerf trigger via string/wire

### 2. Parts Inventory Completed

Reviewed all components from Freenove FNK0047 kit (photos taken and cataloged in `images/`).

**Components identified from kit photos:**
| Component | Model | Status |
|---|---|---|
| Freenove Kit | FNK0047 | Confirmed |
| PIR Motion Sensor | HC-SR501 (HW-416 board, BISS0001 chip) | Have it |
| LED Matrix | 8x8 | Have it (not needed for turret) |
| Potentiometer | B10K | Have it (optional use) |
| 7-Segment Display | Single digit | Have it (not needed) |
| Accelerometer/Gyro | GY-521 (MPU-6050) | Have it (bonus sensor) |
| Active Buzzer | Small black round | Have it |

**Parts to buy (~$54 total):**
- MG996R servos x2 (pan/tilt)
- Pan/tilt metal bracket kit
- Nerf Jolt blaster
- 4xAA battery holder + batteries
- Hot glue gun + zip ties

### 3. Design Document Created

Created comprehensive design document at `ESP32_Sentry_Turret/DESIGN_DOC.md` containing:
- System block diagram and flow chart
- Pin assignments (9 GPIO pins used)
- Full wiring diagram
- State machine design (IDLE → SCANNING → TARGETING → FIRING → COOLDOWN)
- Code architecture (pseudo-code)
- Physical assembly layout
- 14-day working backwards plan (Feb 16 → Mar 1)
- Learning objectives (science, engineering, programming concepts)
- Troubleshooting guide
- Safety rules

### 4. Desktop Shortcut Created

Created "Claude ESP32" shortcut on Desktop that:
- Launches PowerShell with admin privileges
- Changes to `C:\Users\AVENGER\ESP32` directory
- Runs `claude-code --continue` to resume last session

---

## February 18, 2026

### 1. Flash Write-Protection Fix (3rd occurrence)

The Boya flash chip write-protection issue recurred again (status `0x4200`).

**Fix applied:** Same procedure — cleared status register to `0x0200`, erased flash (1.5 seconds).

**Pattern confirmed:** This issue resets on every power cycle. The CMP bit (SR2 bit 6) gets re-latched each time the board loses power.

**Recommendation:** Run `bash ~/ESP32/fix_flash_wp.sh` before every upload session. This is now a standard pre-upload step for this board.

| Occurrence | Date | Status Before | Status After | Erase Time |
|---|---|---|---|---|
| 1st | Feb 16 | 0x4200 | 0x0200 | 1.4s |
| 2nd | Feb 17 | 0x4200 | 0x0200 | 1.5s |
| 3rd | Feb 18 | 0x4200 | 0x0200 | 1.5s |

### 2. Connection Troubleshooting

ESP32 initially failed to connect (`No serial data received`). Resolved by entering download mode manually:
1. Unplug USB
2. Hold BOOT button
3. Plug USB back in while holding BOOT
4. Release BOOT after 2 seconds

---

## Notes & Reminders

- **Always check flash write-protection first** when encountering MD5 mismatch errors on this board
- The `TFT_eSPI` library requires `User_Setup.h` to be configured for the specific TFT display (driver chip, pin mappings, resolution)
- esptool path: `C:\Users\AVENGER\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe`
- Board connects on **COM4**

---

*Log maintained as part of ESP32 Radar project development.*
