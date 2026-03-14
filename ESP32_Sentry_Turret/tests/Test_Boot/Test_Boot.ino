// Servo test - avoiding GPIO 12 (strapping pin!)
// Pan: GPIO 14, Tilt: GPIO 32, Trigger: GPIO 15
#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo1;  // tilt on GPIO 32
Servo servo2;  // pan on GPIO 14
Servo servo3;  // trigger on GPIO 15

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("STEP1_SERIAL_OK");

    servo1.attach(32);
    servo1.write(90);
    delay(500);
    Serial.println("STEP2_TILT_GPIO32_OK");

    servo2.attach(14);
    servo2.write(90);
    delay(500);
    Serial.println("STEP3_PAN_GPIO14_OK");

    servo3.attach(15);
    servo3.write(0);
    delay(500);
    Serial.println("STEP4_TRIGGER_GPIO15_OK");

    Serial.println("ALL_SERVOS_ATTACHED");
}

void loop() {
    Serial.println("PAN_SWEEP_START");
    for (int a = 15; a <= 165; a += 5) {
        servo2.write(a);
        delay(30);
    }
    for (int a = 165; a >= 15; a -= 5) {
        servo2.write(a);
        delay(30);
    }
    servo2.write(90);
    Serial.println("PAN_SWEEP_DONE");

    Serial.println("TILT_TEST");
    servo1.write(70);
    delay(500);
    servo1.write(120);
    delay(500);
    servo1.write(90);
    delay(500);
    Serial.println("TILT_DONE");

    Serial.println("TRIGGER_PULL");
    servo3.write(120);
    delay(400);
    servo3.write(0);
    delay(400);
    Serial.println("TRIGGER_RELEASED");

    Serial.println("CYCLE_COMPLETE");
    delay(3000);
}
