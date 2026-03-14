// Minimal test - find what's crashing
#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("STEP 1: Serial works");

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    Serial.println("STEP 2: LED works");

    servo1.attach(12);  // tilt - this one works
    Serial.println("STEP 3: Servo on GPIO 12 attached");

    servo1.write(90);
    Serial.println("STEP 4: Servo on GPIO 12 moved");

    servo2.attach(14);  // pan
    Serial.println("STEP 5: Servo on GPIO 14 attached");

    servo2.write(90);
    Serial.println("STEP 6: Servo on GPIO 14 moved");

    servo3.attach(15);  // trigger
    Serial.println("STEP 7: Servo on GPIO 15 attached");

    servo3.write(0);
    Serial.println("STEP 8: Servo on GPIO 15 moved");

    Serial.println("ALL STEPS PASSED!");
}

void loop() {
    Serial.println("Running...");
    delay(2000);
}
