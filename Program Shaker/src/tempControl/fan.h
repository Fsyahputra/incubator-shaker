//
// Created by fadhil on 9/6/25.
//

#ifndef FAN_H
#define FAN_H

enum class FanDirection {
    CW,
    CCW,
};

class ShakerFan {
private:
    const int IN1Pin;
    const int IN2Pin;
    const int ENPin; // PWM pin
    const int pwmMaxVal = 1023; // 10 bit resolution
    const unsigned long speedChangeInterval = 500; // milliseconds
    float currentSpeed = 0; // 0-100%
    const float gainConstant = 1.0; // how fast the speed change
    unsigned long lastTimeChange = 0;
    FanDirection currentDirection = FanDirection::CW;
    float currentTemperature = 0.0;
    int currentPwmValue = 0;
    float targetTemperature = 0.0;
    void changeDirection(FanDirection direction);
    void computeNewSpeed();
    void setTargetTemperature(float newTargetTemperature);
    void convertSpeedToPwm();
    void setCw() const;
    void setCcw() const;

public:
    ShakerFan(int IN1Pin, int IN2Pin, int ENPin);
    void init() const;
    void run(float temperature, float targetTemperature);
    void turnOn(FanDirection direction);
    void turnOff() const;
};

#endif //FAN_H