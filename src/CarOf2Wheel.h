#ifndef CAROF2WHEEL_H
#define CAROF2WHEEL_H

#pragma once

#include "TTMotor.h"

#define WHEEL_SPAN 12.5     // cent meter
#define VELOCITY_RATIO 800  // cent meter per second
#define VELOCITY_LIMIT 1200 // cent meter per second

class CarOf2Wheel {
public:
    CarOf2Wheel(int l_in1,
                int l_in2,
                int l_pwm,
                int r_in1,
                int r_in2,
                int r_pwm);
    ~CarOf2Wheel();

    void Start();
    void Stop();

    void Run(uint16_t angle, uint8_t radius);
    void Run2(uint16_t angle, uint8_t radius);

    void DebugPrint();

private:
    TTMotor _leftMotor;
    TTMotor _rightMotor;
    float   _vLeft;
    float   _vRight;

    float calcSpeed(uint8_t radius);
    /**
     * @brief
     *
     * @param angle joystick input 0~359 degree
     * @param region 0:forward  1:backward 2:cw turn 3:ccw turn 4:Q1  5:Q2 6:Q3 7:Q4
     * @return float meter
     */
    float calcRadius(uint16_t angle, uint8_t* region);
    bool  calcDriveSpeed(float radius, float speed, float* left, float* right);
};

#endif