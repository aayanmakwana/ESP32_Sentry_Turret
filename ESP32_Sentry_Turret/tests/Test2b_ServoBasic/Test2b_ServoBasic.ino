// BASIC SERVO TEST - just sweeps one servo back and forth
// If this doesn't work, it's a wiring or power problem

#include <Arduino.h>
#include <ESP32Servo.h>

Servo testServo;

void setup() {
    Serial.begin(115200);
    delay(1000);

    testServo.attach(13);  // Pan servo on GPIO 13
    Serial.println("Servo test starting...");
    Serial.println("Servo should sweep back and forth continuously");
}

void loop() {
    Serial.println("Moving to 0...");
    testServo.write(0);
    delay(1000);

    Serial.println("Moving to 90...");
    testServo.write(90);
    delay(1000);

    Serial.println("Moving to 180...");
    testServo.write(180);
    delay(1000);

    Serial.println("Moving to 90...");
    testServo.write(90);
    delay(1000);
}
