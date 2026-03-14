// Trigger servo test - find the right pull angle
// Type a number 0-180 in Serial Monitor to move the trigger servo
#include <Arduino.h>
#include <ESP32Servo.h>

#define TRIGGER_PIN 4

Servo triggerServo;
int currentAngle = 90;

void setup() {
    Serial.begin(115200);
    delay(2000);

    triggerServo.attach(TRIGGER_PIN);
    triggerServo.write(90);

    Serial.println("=== TRIGGER SERVO TEST ===");
    Serial.println("Type a number 0-180 to move the trigger servo");
    Serial.println("Find which angle PULLS the Nerf trigger");
    Serial.println("Current: 90");
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        int angle = input.toInt();

        if (angle >= 0 && angle <= 180) {
            currentAngle = angle;
            triggerServo.write(180 - angle);
            Serial.printf("Trigger servo -> %d (inverted: %d)\n", angle, 180 - angle);
        } else {
            Serial.println("Enter 0-180");
        }
    }
    delay(10);
}
