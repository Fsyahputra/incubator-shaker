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

  int sdaPin;
  int sclPin;

  float rpm = 0.0;
  float time = 0.0;
  float temperature = 0.0;

  void showConfigTitle();
  void showConfigValue();
  void showRpm();
  void showTime();
  void showTemperature();
  void resetDisplay();
  displayState getState();

public:
  ShakerDisplay(LiquidCrystal_I2C &lcd, int sdaPin, int sclPin);
  void setConfigTitle(const char *title);
  void setConfigValue(const char *label, float value);
  void setRpm(float rpm);
  void setTime(float time);
  void setTemperature(float temperature);
  void run();
  void initDisplay();
  void setState(displayState newState);
};

#endif // DISPLAY_H