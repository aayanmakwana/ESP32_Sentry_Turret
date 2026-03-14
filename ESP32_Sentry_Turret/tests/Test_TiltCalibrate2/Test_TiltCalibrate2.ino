// TILT CALIBRATION v2 - No Serial Monitor needed
// Press the BOOT button on the ESP32 to step through angles
// Watch the servo and remember which angle aims the gun forward
// The current angle is shown by LED blinks AND printed to Serial

#include <Arduino.h>
#include <ESP32Servo.h>

#define TILT_PIN    32
#define BOOT_BTN     0   // BOOT button on ESP32 board
#define BUZZER_PIN  26
#define GREEN_LED    2

Servo tiltServo;

// Angles to test
int angles[] = {60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180};
int numAngles = 13;
int currentIndex = 3;  // start at 90

void beepAngle(int angle) {
    // Beep the number of tens (e.g., 120 = 12 beeps)
    int tens = angle / 10;
    for (int i = 0; i < tens; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(80);
        digitalWrite(BUZZER_PIN, LOW);
        delay(80);
    }
}

void moveToAngle() {
    int angle = angles[currentIndex];
    tiltServo.write(angle);

    Serial.printf(">>> TILT ANGLE: %d degrees  (press BOOT for next) <<<\n", angle);

    // Blink LED to show which angle
    digitalWrite(GREEN_LED, HIGH);
    delay(300);
    digitalWrite(GREEN_LED, LOW);

    // Short beep
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
    Serial.begin(115200);
    delay(2000);  // extra delay to let power stabilize

    pinMode(BOOT_BTN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    tiltServo.attach(TILT_PIN);
    delay(500);

    Serial.println("=== TILT CALIBRATION v2 ===");
    Serial.println("Press BOOT button on ESP32 to step through angles");
    Serial.println("Watch where the gun points at each angle");
    Serial.println("Remember the angle that aims straight forward!\n");

    moveToAngle();
}

void loop() {
    // Wait for BOOT button press
    if (digitalRead(BOOT_BTN) == LOW) {
        delay(300);  // debounce

        // Next angle
        currentIndex++;
        if (currentIndex >= numAngles) {
            currentIndex = 0;  // wrap around
            Serial.println("\n--- Starting over from 60 ---\n");
        }

        moveToAngle();

        // Wait for button release
        while (digitalRead(BOOT_BTN) == LOW) {
            delay(10);
        }
        delay(200);
    }
    delay(10);
}
