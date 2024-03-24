#include <Arduino.h>
#include <Dabble.h>

#include "CarOf2Wheel.h"
#include "TTMotor.h"

#define PWMB A2
#define BIN1 PE5
#define BIN2 PE6
#define PWMA A0
#define AIN1 PA4
#define AIN2 PA5

#define DabbleTx PB10
#define DabbleRx PB11

#define DEBUG
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

HardwareSerial Serial3(PB11, PB10);

CarOf2Wheel car(AIN1, AIN2, PWMA, BIN1, BIN2, PWMB);

uint8_t buf[20];

void setup()
{
    pinMode(LED_GREEN, OUTPUT);
    pinMode(PF9, OUTPUT);

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(PF9, HIGH);

    Dabble.begin(115200, DabbleRx, DabbleTx);
    Serial.begin(115200);
    buf[0] = 20;
    buf[1] = 1;

    TTMotor::SetPWMFrequence(500);
}

void loop()
{
    Dabble.processInput();

    static uint16_t angle;
    uint16_t        nAngle = GamePad.getAngle();
    static uint8_t  radius;
    uint8_t         nRadius = GamePad.getRadius();

    bool start = GamePad.isStartPressed();
    bool stop  = GamePad.isSelectPressed();
    bool debug = GamePad.isTrianglePressed();

    static bool lastDebug = false;

    if (start) {
        digitalWrite(LED_GREEN, LOW);
        car.Start();
    }
    if (stop) {
        digitalWrite(LED_GREEN, HIGH);
        car.Stop();
    }
    if (debug && !lastDebug) {
        car.DebugPrint();
    }
    lastDebug = debug;

    if (angle != nAngle) {
        angle = nAngle;
        Serial.printf("Angle:%d\r\n", angle);
    }

    if (radius != nRadius) {
        radius = nRadius;
        Serial.printf("Radius:%d\r\n", radius);
    }
    car.Run2(angle, radius);
}
