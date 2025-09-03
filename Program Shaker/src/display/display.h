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
  char lastConfigTitle[20] = "";
  char configValueLabel[20] = "";
  char lastConfigValueLabel[20] = "";

  char rpmChar[20];
  char lastRpmChar[20];

  char timeChar[20];
  char lastTimeChar[20];

  char tempChar[20];
  char lastTempChar[20];

  float configValue = 0.0;
  float lastConfigValue = 0.0;

  int sdaPin;
  int sclPin;

  float rpm = 0.0;
  char time[20] = "00:00:00";
  float temperature = 0.0;

  float lastRpm = 0.0;
  char lastTime[20] = "00:00:00";
  float lastTemperature = 0.0;


  void resetToNCloumns(int firstColumn, int lastColumn, int row) const;
  void showConfigTitle();
  void showConfigValue();
  void showRpm();
  void showTime();
  void showTemperature();
  void resetRow(int row) const;
  void resetVariables();


  displayState getState();

public:
  ShakerDisplay(LiquidCrystal_I2C &lcd, int sdaPin, int sclPin);
  void setConfigTitle(const char *title);
  void resetDisplay() const;

  void setConfigValue(const char *label, float value);
  void setRpm(float rpm);
  void setTime(char *time);
  void setTemperature(float temperature);
  void run();
  void initDisplay();
  void setState(displayState newState);
};

#endif // DISPLAY_H