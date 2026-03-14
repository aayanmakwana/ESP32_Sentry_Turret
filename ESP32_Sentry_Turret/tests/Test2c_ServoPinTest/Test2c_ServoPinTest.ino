// SERVO PIN TEST - pan servo on GPIO 13
// Connect MG996R pan servo:
//   Orange → GPIO 13
//   Red    → Battery (+)
//   Brown  → Battery (-) and ESP32 GND

#include <Arduino.h>
#include <ESP32Servo.h>

Servo testServo;

void setup() {
    Serial.begin(115200);
    delay(1000);

    testServo.attach(13);  // Pan servo on GPIO 13
    Serial.println("Testing pan servo on GPIO 13...");
}

void loop() {
    Serial.println("0 degrees");
    testServo.write(0);
    delay(1500);

    Serial.println("90 degrees");
    testServo.write(90);
    delay(1500);

    Serial.println("180 degrees");
    testServo.write(180);
    delay(1500);
}
