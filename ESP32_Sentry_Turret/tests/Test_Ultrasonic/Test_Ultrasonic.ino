// Ultrasonic sensor diagnostic
// Tests GPIO 5 (TRIG) and GPIO 18 (ECHO)
#include <Arduino.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== ULTRASONIC DIAGNOSTIC ===");

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Check ECHO pin idle state
    int echoState = digitalRead(ECHO_PIN);
    Serial.printf("ECHO pin idle state: %d (should be 0)\n", echoState);
    Serial.println();
}

void loop() {
    // Send trigger pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo with timeout
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    long distance = duration * 0.034 / 2.0;

    Serial.printf("Duration: %ld us, Distance: %ld cm", duration, distance);
    if (duration == 0) {
        Serial.print("  << NO ECHO - check wiring!");
        int echo = digitalRead(ECHO_PIN);
        Serial.printf(" ECHO pin=%d", echo);
    }
    Serial.println();

    delay(500);
}
