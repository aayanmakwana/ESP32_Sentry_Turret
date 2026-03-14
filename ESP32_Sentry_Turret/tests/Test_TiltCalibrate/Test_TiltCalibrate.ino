// TILT CALIBRATION - find the right angle for your Nerf gun
// Type an angle number (0-180) in Serial Monitor to move the tilt servo
// Find the angle where the gun points straight at the target

#include <Arduino.h>
#include <ESP32Servo.h>

#define TILT_PIN 32

Servo tiltServo;
int currentAngle = 90;

void setup() {
    Serial.begin(115200);
    delay(1000);

    tiltServo.attach(TILT_PIN);
    tiltServo.write(currentAngle);

    Serial.println("=== TILT CALIBRATION ===");
    Serial.println("Type a number 0-180 to set the tilt angle");
    Serial.println("  0   = fully tilted one way");
    Serial.println("  90  = center");
    Serial.println("  180 = fully tilted other way");
    Serial.println("");
    Serial.println("Try: 90, 100, 110, 120, 130, 140, 150, 160, 170, 180");
    Serial.println("Find the angle where the gun aims straight forward.");
    Serial.printf("Current angle: %d\n\n", currentAngle);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        int angle = input.toInt();

        if (angle >= 0 && angle <= 180) {
            currentAngle = angle;
            tiltServo.write(angle);
            Serial.printf("Tilt → %d°\n", angle);
        } else {
            Serial.println("Enter a number between 0 and 180");
        }
    }
    delay(10);
}
