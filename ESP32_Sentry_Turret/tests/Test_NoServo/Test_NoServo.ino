// Minimal boot test - NO servos, just serial
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("BOOT_OK");
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    Serial.println("LED_ON");
}

void loop() {
    Serial.println("ALIVE");
    delay(2000);
}
