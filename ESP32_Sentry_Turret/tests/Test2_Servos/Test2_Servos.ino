// TEST 2: All 3 Servos (Pan, Tilt, Trigger)
// Open Serial Monitor at 115200 baud
// Type commands to control servos:
//   p90  = move pan servo to 90 degrees
//   t90  = move tilt servo to 90 degrees
//   f    = fire (trigger servo pulls and releases)
//   s    = sweep pan servo back and forth
//   h    = home all servos to center position

#include <Arduino.h>
#include <ESP32Servo.h>

#define PAN_SERVO_PIN     13
#define TILT_SERVO_PIN    32
#define TRIGGER_SERVO_PIN  4

Servo panServo;
Servo tiltServo;
Servo triggerServo;

int panAngle = 90;
int tiltAngle = 90;

void homeAll() {
    panAngle = 90;
    tiltAngle = 90;
    panServo.write(90);
    tiltServo.write(90);
    triggerServo.write(0);
    Serial.println("All servos homed to center");
}

void fire() {
    Serial.println("FIRING!");
    triggerServo.write(90);  // pull trigger
    delay(300);
    triggerServo.write(0);   // release trigger
    delay(200);
    Serial.println("Trigger released");
}

void sweep() {
    Serial.println("Sweeping pan servo 15-165 degrees...");
    for (int a = 15; a <= 165; a += 2) {
        panServo.write(a);
        delay(20);
    }
    for (int a = 165; a >= 15; a -= 2) {
        panServo.write(a);
        delay(20);
    }
    panServo.write(90);
    Serial.println("Sweep done, returned to center");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    panServo.attach(PAN_SERVO_PIN);
    tiltServo.attach(TILT_SERVO_PIN);
    triggerServo.attach(TRIGGER_SERVO_PIN);

    homeAll();

    Serial.println("=== TEST 2: Servos ===");
    Serial.println("Commands:");
    Serial.println("  p[angle]  - pan servo (e.g. p90, p45, p135)");
    Serial.println("  t[angle]  - tilt servo (e.g. t90, t60, t120)");
    Serial.println("  f         - fire trigger");
    Serial.println("  s         - sweep pan left-right");
    Serial.println("  h         - home all servos");
    Serial.println("Ready! Type a command...\n");
}

void loop() {
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.startsWith("p")) {
            int angle = cmd.substring(1).toInt();
            angle = constrain(angle, 0, 180);
            panAngle = angle;
            panServo.write(angle);
            Serial.printf("Pan → %d°\n", angle);
        }
        else if (cmd.startsWith("t")) {
            int angle = cmd.substring(1).toInt();
            angle = constrain(angle, 0, 180);
            tiltAngle = angle;
            tiltServo.write(angle);
            Serial.printf("Tilt → %d°\n", angle);
        }
        else if (cmd == "f") {
            fire();
        }
        else if (cmd == "s") {
            sweep();
        }
        else if (cmd == "h") {
            homeAll();
        }
        else {
            Serial.println("Unknown command. Use p, t, f, s, or h");
        }
    }
    delay(10);
}
