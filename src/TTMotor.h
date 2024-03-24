#ifndef TTMOTOR_H
#define TTMOTOR_H

#pragma once

#include <stdint.h>

#define MAX_SPEED 255

class TTMotor {
public:
    TTMotor(int in1, int in2, int pwm);
    ~TTMotor();

    void Start();
    void Stop();
    void Brake();
    void SetCompensate(float k);
    void SetSpeed(int sp);
    /**
     * @brief Set the Dead Band object
     *
     * @param dead 0~255, if pwm plus width below dead output force is too low, useless
     */
    void SetDeadBand(uint8_t dead);

    void DebugPrint();

    static void SetPWMFrequence(int freq);

private:
    typedef enum {
        DIR_FORWARD,
        DIR_BACKWARD,
        DIR_BRAKE
    } DirType;

    float _compensate;
    int   _deadBand;

    int     _in1;
    int     _in2;
    int     _pwm;
    int     _speed;
    bool    _isStarted;
    DirType _dir;

    /**
     * @brief Set the Dir object
     *
     * @param forward
     */
    void setDir(DirType dir);
};

#endif