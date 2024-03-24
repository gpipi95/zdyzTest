#include "CarOf2Wheel.h"

#include "WMath.h"
#include "WSerial.h"

#include <math.h>

CarOf2Wheel::CarOf2Wheel(int l_in1, int l_in2, int l_pwm, int r_in1, int r_in2, int r_pwm)
    : _leftMotor(l_in1, l_in2, l_pwm)
    , _rightMotor(r_in1, r_in2, r_pwm)
    , _vLeft(0)
    , _vRight(0)
{
}

CarOf2Wheel::~CarOf2Wheel()
{
}

void CarOf2Wheel::Start()
{
    _leftMotor.SetDeadBand(0x30);
    _rightMotor.SetDeadBand(0x30);
    _leftMotor.Start();
    _rightMotor.Start();
}

void CarOf2Wheel::Stop()
{
    _leftMotor.Stop();
    _rightMotor.Stop();
}

void CarOf2Wheel::Run(uint16_t angle, uint8_t radius)
{
    uint8_t region = 0;
    float   speed = 0, R = 0, sign1 = 1, sign2 = 1;

    R     = calcRadius(angle, &region);
    speed = calcSpeed(radius);

    int v1Dir[] = { 1, -1, 1, -1, 1, 1, -1, -1 };
    int v2Dir[] = { 1, -1, -1, 1, 1, 1, -1, -1 };

    if (R > 0) {
        calcDriveSpeed(R, speed, &_vLeft, &_vRight);
    } else {
        _vLeft  = speed * 3 / 4;
        _vRight = _vLeft;
    }
    region  = region < 7 ? region : 7;
    _vLeft  = v1Dir[region] * _vLeft;
    _vRight = v2Dir[region] * _vRight;

    _leftMotor.SetSpeed((int)map(_vLeft,
                                 -1 * VELOCITY_LIMIT, VELOCITY_LIMIT,
                                 -1 * MAX_SPEED, MAX_SPEED));
    _rightMotor.SetSpeed((int)map(_vRight,
                                  -1 * VELOCITY_LIMIT, VELOCITY_LIMIT,
                                  -1 * MAX_SPEED, MAX_SPEED));
}

void CarOf2Wheel::Run2(uint16_t angle, uint8_t radius)
{
    float speed = radius;

    uint8_t region = 0;

    int vLDir[] = { 1, -1, 1, -1, 1, 1, -1, -1 };
    int vRDir[] = { 1, -1, -1, 1, 1, 1, -1, -1 };

    calcRadius(angle, &region);
    switch (region) {
    case 0:
    case 1:
        _vLeft  = speed;
        _vRight = speed;
        break;
    case 2:
    case 3:
        _vLeft  = speed * 0.6;
        _vRight = speed * 0.6;
        break;
    case 4: // Q1
    case 7: // Q4
        _vLeft  = speed;
        _vRight = speed * sinf(angle);
        break;
    case 5: // Q2
    case 6: // Q3
        _vLeft  = speed * sinf(angle);
        _vRight = speed;
        break;
    default:
        _vLeft  = 0;
        _vRight = 0;
        break;
    }
    _vLeft  = copysignf(_vLeft, vLDir[region]);
    _vRight = copysignf(_vRight, vRDir[region]);

    _leftMotor.SetSpeed((int)map(_vLeft,
                                 -7, 7,
                                 -1 * MAX_SPEED, MAX_SPEED));
    _rightMotor.SetSpeed((int)map(_vRight,
                                  -7, 7,
                                  -1 * MAX_SPEED, MAX_SPEED));
}

void CarOf2Wheel::DebugPrint()
{
    Serial.printf("L:%d,R:%d\r\n", (int)_vLeft, (int)_vRight);
    _leftMotor.DebugPrint();
    _rightMotor.DebugPrint();
}

float CarOf2Wheel::calcSpeed(uint8_t radius)
{
    return map(radius, 0, 7, 0, VELOCITY_RATIO);
}

float CarOf2Wheel::calcRadius(uint16_t angle, uint8_t* region)
{
    if (angle > 350 || angle <= 10) {
        *region = 2;
        return 0;
    }
    if (angle > 80 && angle <= 100) {
        *region = 0;
        return 0;
    }
    if (angle > 170 && angle <= 190) {
        *region = 3;
        return 0;
    }
    if (angle > 260 && angle <= 280) {
        *region = 1;
        return 0;
    }

    if (angle > 0 && angle < 90) {
        *region = 4;
    } else if (angle > 90 && angle < 180) {
        *region = 5;
    } else if (angle > 180 && angle < 270) {
        *region = 6;
    } else if (angle > 270 && angle < 360) {
        *region = 7;
    }
    return 0;
}
bool CarOf2Wheel::calcDriveSpeed(float radius, float speed, float* left, float* right)
{
    float deta = (WHEEL_SPAN * speed) / 2 / radius;
    *left      = speed + deta;
    *right     = speed - deta;
    return radius != 0;
}
