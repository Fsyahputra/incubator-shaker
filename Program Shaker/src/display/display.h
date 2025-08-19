#ifndef DISPLAY_H
#define DISPLAY_H
#include <LiquidCrystal_I2C.h>

enum class displayState
{
  CONFIGURING,
  READY,
};

class ShakerDisplay
{

private:
  LiquidCrystal_I2C &lcd;
  displayState state = displayState::CONFIGURING;
  char configTitle[20] = "Configuring";
  char configValueLabel[20] = "";
  float configValue = 0.0;

  float rpm = 0.0;
  float time = 0.0;
  float temperature = 0.0;

  void showConfigTitle();
  void showConfigValue();
  void showRpm();
  void showTime();
  void showTemperature();
  void initDisplay();
  void resetDisplay();
  displayState getState();

public:
  ShakerDisplay(LiquidCrystal_I2C &lcd);
  void setConfigTitle(const char *title);
  void setConfigValue(const char *label, float value);
  void setRpm(float rpm);
  void setTime(float time);
  void setTemperature(float temperature);
  void run();
  void setState(displayState newState);
};

#endif // DISPLAY_H