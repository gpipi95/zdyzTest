#include "TTMotor.h"

#include "WSerial.h"
#include "wiring_analog.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "wiring_time.h"

#include <algorithm>

TTMotor::TTMotor(int in1, int in2, int pwm)
    : _compensate(1)
    , _deadBand(5)
    , _in1(in1)
    , _in2(in2)
    , _pwm(pwm)
    , _speed(0)
    , _isStarted(false)
    , _dir(DIR_FORWARD)
{
}

TTMotor::~TTMotor()
{
}

void TTMotor::Start()
{
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    setDir(DIR_BRAKE);
    _isStarted = true;
}

void TTMotor::Stop()
{
    setDir(DIR_BRAKE);
    _isStarted = false;
}

void TTMotor::Brake()
{
    setDir(DIR_BRAKE);
}

void TTMotor::SetCompensate(float k)
{
    _compensate = k;
}

void TTMotor::SetSpeed(int sp)
{
    if (!_isStarted) {
        return;
    }
    DirType dir = (sp > 0 ? DIR_FORWARD : DIR_BACKWARD);
    sp          = std::min((uint8_t)MAX_SPEED, (uint8_t)abs(_compensate * sp));
    if (sp < _deadBand) {
        _speed = 0;
        Brake();
    } else {
        setDir(dir);
        if (_speed != sp) {
            if (_speed == 0) {
                analogWrite(_pwm, 255);
                delay(50);
            }
            _speed = sp;
            analogWrite(_pwm, _speed);
        }
    }
}

void TTMotor::SetDeadBand(uint8_t dead)
{
    _deadBand = dead;
}

void TTMotor::DebugPrint()
{
    Serial1.printf("sp:%d\tstart:%d\tforward:%d\r\n", _speed, _isStarted, _dir);
}

void TTMotor::SetPWMFrequence(int freq)
{
    analogWriteFrequency(freq);
}

void TTMotor::setDir(DirType dir)
{
    switch (dir) {
    case DIR_BACKWARD:
        if (_dir != DIR_BACKWARD) {
            digitalWrite(_in1, LOW);
            digitalWrite(_in2, HIGH);
            _dir = DIR_BACKWARD;
        }
        break;
    case DIR_FORWARD:
        if (_dir != DIR_FORWARD) {
            digitalWrite(_in1, HIGH);
            digitalWrite(_in2, LOW);
            _dir = DIR_FORWARD;
        }
        break;
    default:
        if (_dir != DIR_BRAKE) {
            digitalWrite(_in1, LOW);
            digitalWrite(_in2, LOW);
            _dir = DIR_BRAKE;
        }
        break;
    }
}
