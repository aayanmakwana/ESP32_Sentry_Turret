#include <dummy.h>

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ESP32Servo.h>
#include <cmath>

TFT_eSPI tft;
Servo radarServo;

constexpr int SERVO_PIN = 13;
constexpr int TRIG_PIN  = 5;
constexpr int ECHO_PIN  = 18;

constexpr int centerX = 160;
constexpr int centerY = 220;
constexpr int radius  = 150;

long readDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    return static_cast<long>(duration * 0.034 / 2.0);
}

void drawRadar() {
    tft.drawCircle(centerX, centerY, radius, TFT_GREEN);
    tft.drawCircle(centerX, centerY, radius * 0.75, TFT_GREEN);
    tft.drawCircle(centerX, centerY, radius * 0.5, TFT_GREEN);
    tft.drawCircle(centerX, centerY, radius * 0.25, TFT_GREEN);
}

void drawSweep(int angle, long dist) {
    float rad = angle * PI / 180.0;

    int x = centerX + radius * cos(rad);
    int y = centerY - radius * sin(rad);

    tft.drawLine(centerX, centerY, x, y, TFT_GREEN);

    if (dist < 200) {
        int objX = centerX + dist * cos(rad);
        int objY = centerY - dist * sin(rad);
        tft.fillCircle(objX, objY, 4, TFT_RED);
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    radarServo.attach(SERVO_PIN);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    drawRadar();
}

void loop() {
    for (int angle = 15; angle <= 165; angle++) {
        radarServo.write(angle);
        delay(30);
        long dist = readDistance();
        drawSweep(angle, dist);
    }

    for (int angle = 165; angle >= 15; angle--) {
        radarServo.write(angle);
        delay(30);
        long dist = readDistance();
        drawSweep(angle, dist);
    }
}
