//
// Created by fadhil on 9/6/25.
//

#ifndef PELTIER_H
#define PELTIER_H
#include "tempControl/tempSensorIF.h"

class ShakerPeltier {

private:
    int peltierPin;
    const float maxTemperature;
    tempIF &sensor;
    float currentTemperature = 0.0;
    const unsigned long readInterval = 5000;
    unsigned long lastRead = 0;
    void readTempWithInterval();
public:
    ShakerPeltier(int pin,  tempIF &sensor);
    void init() const;
    void run();
    void turnOn() const;
    void turnOff() const;
};

#endif //PELTIER_H