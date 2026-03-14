// TEST 3: Scanning - Ultrasonic sensor mounted on pan servo
// This combines servo sweep with distance reading
// It scans the area and reports the closest object's angle and distance
// Open Serial Monitor at 115200 baud

#include <Arduino.h>
#include <ESP32Servo.h>

#define PAN_SERVO_PIN  13
#define TRIG_PIN        5
#define ECHO_PIN       18
#define GREEN_LED       2
#define BUZZER_PIN     26

Servo panServo;

long readDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duration == 0) return 999;
    return duration * 0.034 / 2.0;
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    panServo.attach(PAN_SERVO_PIN);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    panServo.write(90);
    delay(500);

    Serial.println("=== TEST 3: Scan & Detect ===");
    Serial.println("Place an object (book, box) in front of the turret");
    Serial.println("Scanning 15° to 165°...\n");
}

void loop() {
    int closestAngle = -1;
    long closestDist = 999;

    digitalWrite(GREEN_LED, HIGH);
    Serial.println("--- SCANNING ---");

    // Sweep forward
    for (int angle = 15; angle <= 165; angle += 3) {
        panServo.write(angle);
        delay(50);
        long dist = readDistance();

        // Print a simple radar-style display
        Serial.printf("  %3d° : %3ld cm ", angle, dist);
        int bars = map(constrain(dist, 0, 200), 0, 200, 20, 0);
        for (int i = 0; i < bars; i++) Serial.print("█");
        if (dist < 150) Serial.print(" ◄ TARGET");
        Serial.println();

        if (dist < closestDist && dist > 2) {
            closestDist = dist;
            closestAngle = angle;
        }
    }

    // Report result
    Serial.println();
    if (closestAngle >= 0 && closestDist < 150) {
        Serial.printf(">>> TARGET FOUND at %d°, distance %ld cm <<<\n\n", closestAngle, closestDist);
        digitalWrite(GREEN_LED, LOW);

        // Aim at target
        panServo.write(closestAngle);
        Serial.printf("Aiming at %d°...\n", closestAngle);

        // Beep 3 times
        for (int i = 0; i < 3; i++) {
            digitalWrite(BUZZER_PIN, HIGH);
            delay(100);
            digitalWrite(BUZZER_PIN, LOW);
            delay(100);
        }

        delay(2000); // hold on target for 2 seconds
    } else {
        Serial.println(">>> No target in range <<<\n");
    }

    delay(1000);
}
