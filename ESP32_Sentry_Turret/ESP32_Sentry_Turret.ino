// =====================================================
// ESP32 AUTOMATED SENTRY TURRET
// Science Fair 2026 - Grade 8
// =====================================================
//
// WIRING:
//   Pan Servo (MG996R):     Orange → GPIO 13,  Power from battery pack
//   Tilt Servo (MG996R):    Orange → GPIO 32,  Power from battery pack
//   Trigger Servo (SG90):   Orange → GPIO 4,   Red → ESP32 5V, Brown → GND
//   Ultrasonic TRIG:        GPIO 5
//   Ultrasonic ECHO:        GPIO 18
//   PIR Motion Sensor:      GPIO 27
//   Buzzer:                 GPIO 26
//   Green LED (armed):      GPIO 2
//   Battery (-) must connect to ESP32 GND (common ground)
//
// SERIAL MONITOR COMMANDS (115200 baud, Newline):
//   D = Check distance sensor
//   M = Simulate motion trigger (force start scanning)
//   F = Force fire
//   R = Reset to IDLE
//   P = Print current state and settings
//   T[angle] = Manually set tilt (e.g. T120)
//   A = Toggle auto/manual mode

#include <Arduino.h>
#include <ESP32Servo.h>

// ==================== PIN DEFINITIONS ====================
#define PAN_SERVO_PIN     13
#define TILT_SERVO_PIN    32
#define TRIGGER_SERVO_PIN  4
#define TRIG_PIN           5
#define ECHO_PIN          18
#define PIR_PIN           27
#define BUZZER_PIN        26
#define GREEN_LED_PIN      2

// ==================== TUNING CONSTANTS ====================
// Adjust these to calibrate your turret

// Pan servo scan range (degrees)
#define PAN_MIN           15
#define PAN_MAX          165
#define PAN_CENTER        90
#define PAN_STEP           3    // degrees per scan step
#define PAN_DELAY         40    // ms between scan steps

// Tilt servo positions - ADJUST THESE for your Nerf gun mounting
#define TILT_LEVEL        90    // gun aims straight (change this after calibration!)
#define TILT_UP           70    // gun aims up (for far targets)
#define TILT_DOWN        120    // gun aims down (for close targets)

// Trigger servo
#define TRIGGER_REST     180    // not pulling
#define TRIGGER_PULL       0    // pulling trigger
#define TRIGGER_HOLD_MS  400    // how long to hold trigger pulled

// Detection
#define MAX_DETECT_DIST  150    // max detection distance in cm
#define MIN_DETECT_DIST    3    // ignore readings closer than this
#define PIR_WARMUP_SEC    10    // PIR warmup time in seconds

// Timing
#define COOLDOWN_MS     3000    // pause after firing before scanning again
#define SCAN_TIMEOUT_MS 8000    // return to idle if no target found in this time
#define AIM_SETTLE_MS    500    // time to let servos settle after aiming

// ==================== STATE MACHINE ====================
enum State {
    IDLE,
    SCANNING,
    TARGETING,
    FIRING,
    COOLDOWN
};

const char* stateNames[] = {"IDLE", "SCANNING", "TARGETING", "FIRING", "COOLDOWN"};

State currentState = IDLE;

// ==================== SERVO OBJECTS ====================
Servo panServo;
Servo tiltServo;
Servo triggerServo;

// ==================== VARIABLES ====================
int targetAngle = PAN_CENTER;
long targetDistance = 0;
unsigned long stateStartTime = 0;
bool autoMode = true;

// ==================== DISTANCE READING ====================
long readDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout
    if (duration == 0) return 999;  // no echo
    return duration * 0.034 / 2.0;
}

// ==================== SCANNING ====================
// Sweeps the pan servo and looks for the closest object
// Returns true if target found, false if nothing detected
bool scanForTarget() {
    long closestDist = 999;
    int closestAngle = -1;

    Serial.println("  Scanning...");

    // Sweep forward
    for (int angle = PAN_MIN; angle <= PAN_MAX; angle += PAN_STEP) {
        panServo.write(angle);
        delay(PAN_DELAY);

        long dist = readDistance();

        // Print scan data
        Serial.printf("    %3d° : %3ld cm", angle, dist);
        if (dist >= MIN_DETECT_DIST && dist <= MAX_DETECT_DIST) {
            Serial.print("  ◄◄◄");
        }
        Serial.println();

        // Track closest object
        if (dist >= MIN_DETECT_DIST && dist <= MAX_DETECT_DIST) {
            if (dist < closestDist) {
                closestDist = dist;
                closestAngle = angle;
            }
        }

        // Check timeout
        if (millis() - stateStartTime > SCAN_TIMEOUT_MS) {
            Serial.println("  Scan timeout — no target found");
            return false;
        }
    }

    if (closestAngle >= 0) {
        targetAngle = closestAngle;
        targetDistance = closestDist;
        Serial.printf("  >>> TARGET at %d°, %ld cm <<<\n", targetAngle, targetDistance);
        return true;
    }

    return false;
}

// ==================== AIMING ====================
void aimAtTarget() {
    // Move pan to target angle
    panServo.write(targetAngle);

    // Adjust tilt based on distance
    // Closer targets = aim slightly down, far targets = aim slightly up
    int tiltAngle;
    if (targetDistance < 50) {
        tiltAngle = TILT_DOWN;  // close — aim down
    } else if (targetDistance > 120) {
        tiltAngle = TILT_UP;    // far — aim up
    } else {
        tiltAngle = TILT_LEVEL; // medium — aim level
    }

    tiltServo.write(tiltAngle);
    Serial.printf("  Aiming: pan=%d°, tilt=%d°\n", targetAngle, tiltAngle);

    delay(AIM_SETTLE_MS);  // let servos reach position
}

// ==================== FIRING ====================
void fire() {
    Serial.println("  FIRE!");

    // Warning beeps
    for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
    }

    // Long tone + pull trigger
    digitalWrite(BUZZER_PIN, HIGH);
    triggerServo.write(TRIGGER_PULL);
    delay(TRIGGER_HOLD_MS);

    // Release trigger
    triggerServo.write(TRIGGER_REST);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);

    Serial.println("  Trigger released");
}

// ==================== BUZZER PATTERNS ====================
void beepAlert() {
    // Two short beeps
    for (int i = 0; i < 2; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(50);
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);
    }
}

void beepScanning() {
    // Single tick
    digitalWrite(BUZZER_PIN, HIGH);
    delay(20);
    digitalWrite(BUZZER_PIN, LOW);
}

// ==================== STATE CHANGE ====================
void changeState(State newState) {
    Serial.printf("\n[%s] → [%s]\n", stateNames[currentState], stateNames[newState]);
    currentState = newState;
    stateStartTime = millis();
}

// ==================== SERIAL COMMANDS ====================
void handleSerialCommand() {
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "D") {
        long dist = readDistance();
        Serial.printf("Distance: %ld cm\n", dist);
    }
    else if (cmd == "M") {
        Serial.println("Manual trigger — starting scan");
        changeState(SCANNING);
    }
    else if (cmd == "F") {
        Serial.println("Manual fire!");
        changeState(FIRING);
    }
    else if (cmd == "R") {
        Serial.println("Reset to IDLE");
        panServo.write(PAN_CENTER);
        tiltServo.write(TILT_LEVEL);
        triggerServo.write(TRIGGER_REST);
        changeState(IDLE);
    }
    else if (cmd == "P") {
        Serial.printf("State: %s\n", stateNames[currentState]);
        Serial.printf("Target: angle=%d°, dist=%ld cm\n", targetAngle, targetDistance);
        Serial.printf("Auto mode: %s\n", autoMode ? "ON" : "OFF");
        Serial.printf("Tilt level: %d°\n", TILT_LEVEL);
    }
    else if (cmd.startsWith("T")) {
        int angle = cmd.substring(1).toInt();
        angle = constrain(angle, 0, 180);
        tiltServo.write(angle);
        Serial.printf("Tilt → %d°\n", angle);
    }
    else if (cmd == "A") {
        autoMode = !autoMode;
        Serial.printf("Auto mode: %s\n", autoMode ? "ON" : "OFF");
        if (!autoMode) {
            changeState(IDLE);
        }
    }
    else {
        Serial.println("Commands: D=distance, M=scan, F=fire, R=reset, P=status, T[angle]=tilt, A=auto toggle");
    }
}

// ==================== SETUP ====================
void setup() {
    Serial.begin(115200);
    delay(1000);

    // Pin modes
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    // Attach servos
    panServo.attach(PAN_SERVO_PIN);
    tiltServo.attach(TILT_SERVO_PIN);
    triggerServo.attach(TRIGGER_SERVO_PIN);

    // Home position
    panServo.write(PAN_CENTER);
    tiltServo.write(TILT_LEVEL);
    triggerServo.write(TRIGGER_REST);

    Serial.println("=============================================");
    Serial.println("   ESP32 AUTOMATED SENTRY TURRET");
    Serial.println("   Science Fair 2026");
    Serial.println("=============================================");
    Serial.println("");

    // PIR warmup
    Serial.printf("PIR sensor warming up (%d seconds)...\n", PIR_WARMUP_SEC);
    for (int i = PIR_WARMUP_SEC; i > 0; i--) {
        Serial.printf("  %d...\n", i);
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(500);
        digitalWrite(GREEN_LED_PIN, LOW);
        delay(500);
    }

    Serial.println("\nSYSTEM ARMED — Waiting for motion...");
    Serial.println("Commands: D=distance, M=scan, F=fire, R=reset, P=status, T[angle]=tilt, A=auto toggle\n");

    digitalWrite(GREEN_LED_PIN, HIGH);
    changeState(IDLE);
}

// ==================== MAIN LOOP ====================
void loop() {
    // Always check for serial commands
    handleSerialCommand();

    switch (currentState) {

        // -------- IDLE: Waiting for motion --------
        case IDLE:
            digitalWrite(GREEN_LED_PIN, HIGH);

            if (autoMode && digitalRead(PIR_PIN) == HIGH) {
                Serial.println("\n*** MOTION DETECTED! ***");
                beepAlert();
                changeState(SCANNING);
            }
            break;

        // -------- SCANNING: Sweep and find target --------
        case SCANNING:
            digitalWrite(GREEN_LED_PIN, LOW);
            beepScanning();

            if (scanForTarget()) {
                changeState(TARGETING);
            } else {
                Serial.println("  No target — returning to IDLE");
                panServo.write(PAN_CENTER);
                changeState(IDLE);
            }
            break;

        // -------- TARGETING: Aim at target --------
        case TARGETING:
            digitalWrite(GREEN_LED_PIN, LOW);

            aimAtTarget();

            // Verify target is still there
            long verifyDist;
            verifyDist = readDistance();
            Serial.printf("  Verify distance: %ld cm\n", verifyDist);

            if (verifyDist >= MIN_DETECT_DIST && verifyDist <= MAX_DETECT_DIST) {
                Serial.println("  Target confirmed — FIRING");
                changeState(FIRING);
            } else {
                Serial.println("  Target lost — re-scanning");
                changeState(SCANNING);
            }
            break;

        // -------- FIRING: Pull trigger --------
        case FIRING:
            fire();
            changeState(COOLDOWN);
            break;

        // -------- COOLDOWN: Pause before next cycle --------
        case COOLDOWN:
            Serial.printf("  Cooldown %d seconds...\n", COOLDOWN_MS / 1000);
            Serial.println("  (Reload Nerf dart now!)");
            digitalWrite(GREEN_LED_PIN, LOW);

            // Return to center
            panServo.write(PAN_CENTER);
            tiltServo.write(TILT_LEVEL);

            delay(COOLDOWN_MS);

            Serial.println("  Ready again\n");
            changeState(IDLE);
            break;
    }

    delay(50);
}
