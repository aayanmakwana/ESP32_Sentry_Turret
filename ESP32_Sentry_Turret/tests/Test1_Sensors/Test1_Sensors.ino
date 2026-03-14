// TEST 1: PIR Motion Sensor + Ultrasonic Distance Sensor
// Open Serial Monitor at 115200 baud to see output
// Walk in front of the PIR and move your hand in front of the ultrasonic sensor

#include <Arduino.h>

#define PIR_PIN    27
#define TRIG_PIN    5
#define ECHO_PIN   18
#define GREEN_LED   2
#define RED_LED     4
#define BUZZER_PIN 26

long readDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
    if (duration == 0) return -1; // no echo
    return duration * 0.034 / 2.0;
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.println("=== TEST 1: Sensors ===");
    Serial.println("Waiting 30 seconds for PIR to warm up...");
    Serial.println("(PIR needs time to stabilize)");

    // Blink green LED during warmup
    for (int i = 30; i > 0; i--) {
        Serial.printf("  %d seconds remaining...\n", i);
        digitalWrite(GREEN_LED, HIGH);
        delay(500);
        digitalWrite(GREEN_LED, LOW);
        delay(500);
    }

    Serial.println("PIR ready! Starting sensor test...\n");
    digitalWrite(GREEN_LED, HIGH); // solid green = ready
}

void loop() {
    // Read PIR
    int motion = digitalRead(PIR_PIN);

    // Read ultrasonic
    long distance = readDistance();

    // Display results
    Serial.printf("PIR: %s  |  Distance: ", motion ? "MOTION!" : "clear  ");
    if (distance < 0) {
        Serial.println("no echo");
    } else {
        Serial.printf("%ld cm\n", distance);
    }

    // LED indicators
    if (motion) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(50);
        digitalWrite(BUZZER_PIN, LOW);
    } else {
        digitalWrite(RED_LED, LOW);
    }

    delay(300);
}
