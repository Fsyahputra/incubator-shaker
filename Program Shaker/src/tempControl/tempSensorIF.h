//
// Created by fadhil on 9/4/25.
//

#ifndef TEMPSENSORIF_H
#define TEMPSENSORIF_H
#include <Adafruit_MCP9808.h>


class tempIF {
public:
    virtual ~tempIF() = default;
    virtual void init();
    virtual float readTemperatureCelsius() = 0;
};


class Mcp9808Impl final : public tempIF {
private:
    Adafruit_MCP9808 sensor;
    uint8_t i2cAddress;
    uint8_t resolution = 3; // 0=0.5C, 1=0.25C, 2=0.125C, 3=0.0625C

public:
    explicit Mcp9808Impl(const Adafruit_MCP9808 &sensor, uint8_t i2cAddress = 0x18);
    void init() override;
    float readTemperatureCelsius() override;
};

#endif //TEMPSENSORIF_H


