// TEST ALL SERVOS
//
// WIRING:
//   SG90 (trigger):   Orange → GPIO 4,   Red → ESP32 5V,     Brown → ESP32 GND
//   MG996R (pan):     Orange → GPIO 13,  Red → Battery (+),  Brown → Battery (-)
//   MG996R (tilt):    Orange → GPIO 32,  Red → Battery (+),  Brown → Battery (-)
//   Battery (-) ────→ ESP32 GND   *** MUST CONNECT - common ground! ***
//
// Commands in Serial Monitor (set to "Newline"):
//   1 = Test trigger servo (stronger pull)
//   2 = Test pan servo (MG996R)
//   3 = Test tilt servo (MG996R)
//   4 = Test ALL servos at once

#include <Arduino.h>
#include <ESP32Servo.h>

#define TRIGGER_PIN   4
#define PAN_PIN      13
#define TILT_PIN     32

Servo triggerServo;
Servo panServo;
Servo tiltServo;

void testTrigger() {
    Serial.println("\n--- TRIGGER TEST ---");
    Serial.println("Pulling trigger...");

    // Try increasing angles to find what fires the Nerf gun
    for (int angle = 0; angle <= 180; angle += 30) {
        triggerServo.write(angle);
        Serial.printf("  Trigger at %d degrees - did it fire? (watch the gun)\n", angle);
        delay(1000);
    }

    Serial.println("Releasing trigger...");
    triggerServo.write(0);
    delay(500);
    Serial.println("Done. Remember which angle fires the gun!\n");
}

void testPan() {
    Serial.println("\n--- PAN SERVO TEST (MG996R on GPIO 13) ---");
    Serial.println("If nothing moves, check battery pack is ON and (-) connects to ESP32 GND");

    Serial.println("  Center (90)...");
    panServo.write(90);
    delay(1500);

    Serial.println("  Left (15)...");
    panServo.write(15);
    delay(1500);

    Serial.println("  Right (165)...");
    panServo.write(165);
    delay(1500);

    Serial.println("  Center (90)...");
    panServo.write(90);
    delay(500);
    Serial.println("Done.\n");
}

void testTilt() {
    Serial.println("\n--- TILT SERVO TEST (MG996R on GPIO 12) ---");
    Serial.println("If nothing moves, check battery pack is ON and (-) connects to ESP32 GND");

    Serial.println("  Level (90)...");
    tiltServo.write(90);
    delay(1500);

    Serial.println("  Up (60)...");
    tiltServo.write(60);
    delay(1500);

    Serial.println("  Down (120)...");
    tiltServo.write(120);
    delay(1500);

    Serial.println("  Level (90)...");
    tiltServo.write(90);
    delay(500);
    Serial.println("Done.\n");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    triggerServo.attach(TRIGGER_PIN);
    panServo.attach(PAN_PIN);
    tiltServo.attach(TILT_PIN);

    triggerServo.write(0);
    panServo.write(90);
    tiltServo.write(90);

    Serial.println("=== ALL SERVO TEST ===");
    Serial.println("");
    Serial.println("WIRING CHECK:");
    Serial.println("  SG90 trigger:  Orange→GPIO 4,  Red→ESP32 5V, Brown→ESP32 GND");
    Serial.println("  MG996R pan:    Orange→GPIO 13, Red→Battery(+), Brown→Battery(-)");
    Serial.println("  MG996R tilt:   Orange→GPIO 32, Red→Battery(+), Brown→Battery(-)");
    Serial.println("  Battery(-)  →  ESP32 GND  (CRITICAL!)");
    Serial.println("");
    Serial.println("Commands:");
    Serial.println("  1 = Test trigger (SG90)");
    Serial.println("  2 = Test pan (MG996R)");
    Serial.println("  3 = Test tilt (MG996R)");
    Serial.println("  4 = Test ALL");
    Serial.println("");
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();

        if (cmd == '1') testTrigger();
        else if (cmd == '2') testPan();
        else if (cmd == '3') testTilt();
        else if (cmd == '4') {
            testTrigger();
            testPan();
            testTilt();
            Serial.println("=== ALL TESTS COMPLETE ===\n");
        }
    }
    delay(10);
}
