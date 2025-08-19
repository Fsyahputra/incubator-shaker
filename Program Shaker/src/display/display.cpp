#include <LiquidCrystal_I2C.h>
#include <display/display.h>
#include <constant/hardware.h>

void ShakerDisplay::initDisplay()
{
  this->lcd.begin(Hardware::LcdI2C::LCD_COLS, Hardware::LcdI2C::LCD_ROWS);
  this->lcd.backlight();
  this->resetDisplay();
}

displayState ShakerDisplay::getState()
{
  return this->state;
}

void ShakerDisplay::setState(displayState newState)
{
  this->state = newState;
}

ShakerDisplay::ShakerDisplay(LiquidCrystal_I2C &lcd) : lcd(lcd)
{
  initDisplay();
}

void ShakerDisplay::setRpm(float rpm)
{
  this->rpm = rpm;
}

void ShakerDisplay::setConfigTitle(const char *title)
{
  strncpy(this->configTitle, title, sizeof(this->configTitle) - 1);
  this->configTitle[sizeof(this->configTitle) - 1] = '\0';
}

void ShakerDisplay::setConfigValue(const char *label, float value)
{
  strncpy(this->configValueLabel, label, sizeof(this->configValueLabel) - 1);
  this->configValueLabel[sizeof(this->configValueLabel) - 1] = '\0';
  this->configValue = value;
}

void ShakerDisplay::showConfigTitle()
{
  this->lcd.setCursor(0, 0);
  this->lcd.print(this->configTitle);
}

void ShakerDisplay::showConfigValue()
{
  this->lcd.setCursor(0, 1);
  this->lcd.print(this->configValueLabel);
  this->lcd.print(": ");
  this->lcd.print(this->configValue, 2); // Print with 2 decimal places
}

void ShakerDisplay::showRpm()
{
  this->lcd.setCursor(0, 0);
  this->lcd.print("RPM: ");
  this->lcd.print(this->rpm, 2);
}

void ShakerDisplay::showTime()
{
  this->lcd.setCursor(0, 1);
  this->lcd.print("Time: ");
  this->lcd.print(this->time, 2);
}

void ShakerDisplay::showTemperature()
{
  this->lcd.setCursor(8, 0);
  this->lcd.print("Temp: ");
  this->lcd.print(this->temperature, 2);
}

void ShakerDisplay::setTemperature(float temperature)
{
  this->temperature = temperature;
}

void ShakerDisplay::setTime(float time)
{
  this->time = time;
}

void ShakerDisplay::resetDisplay()
{
  this->lcd.clear();
  this->lcd.setCursor(0, 0);
}

void ShakerDisplay::run()
{
  switch (this->getState())
  {
  case displayState::CONFIGURING:
    this->resetDisplay();
    showConfigTitle();
    showConfigValue();
    break;
  case displayState::READY:
    this->resetDisplay();
    showRpm();
    showTime();
    showTemperature();
    break;
  }
}
