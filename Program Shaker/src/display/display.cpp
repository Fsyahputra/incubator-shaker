#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <display/display.h>
#include <constant/hardware.h>
#include <Wire.h>

void ShakerDisplay::initDisplay()
{

  Wire.begin(sdaPin, sclPin);
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

ShakerDisplay::ShakerDisplay(LiquidCrystal_I2C &lcd, int sdaPin, int sclPin) : lcd(lcd), sdaPin(sdaPin), sclPin(sclPin)
{
  this->lcd = lcd;
  this->sdaPin = sdaPin;
  this->sclPin = sclPin;
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
  if (strcmp(this->lastConfigTitle, this->configTitle) == 0) {
    return;
  }
  this->resetRow(0);
  this->lcd.print(this->configTitle);
  strcpy(this->lastConfigTitle, this->configTitle);
}

void ShakerDisplay::showConfigValue()
{
  if (strcmp(this->lastConfigValueLabel, this->configValueLabel)==0 && this->configValue == this->lastConfigValue) {
    return;
  }
  this->resetRow(1);
  this->lcd.setCursor(0, 1);
  this->lcd.print(this->configValueLabel);
  this->lcd.print(": ");
  this->lcd.print(this->configValue, 2);
  strcpy(this->lastConfigValueLabel, this->configValueLabel);
  this->lastConfigValue = this->configValue;
}

void ShakerDisplay::showRpm() {
  if (this->lastRpm == this->rpm) {
    return;
  }
  // this->resetToNCloumns(0, 8, 0);
  this->lcd.setCursor(0, 0);
  this->lcd.print("RPM:");
  this->lcd.print(this->rpm, 2);
  this->lastRpm = this->rpm;
}

void ShakerDisplay::showTime() {
  if (strcmp(this->lastTime, this->time) == 0) {
    return;
  }
  this->resetRow(1);
  this->lcd.setCursor(0, 1);
  this->lcd.print("Time:");
  this->lcd.print(this->time);
  strcpy(this->lastTime, this->time);
}

void ShakerDisplay::showTemperature() {
  if (this->lastTemperature == this->temperature) {
    return;
  }
  // this->resetToNCloumns(8, 16, 0);
  this->lcd.setCursor(8, 0);
  this->lcd.print("Temp:");
  this->lcd.print(this->temperature, 2);
  this->lastTemperature = this->temperature;
}

void ShakerDisplay::setTemperature(float temperature)
{
  this->temperature = temperature;
}

void ShakerDisplay::setTime(char *time)
{
  strcpy(this->time, time);
}

void ShakerDisplay::resetDisplay() const {
  this->lcd.clear();
  this->lcd.setCursor(0, 0);
}


void ShakerDisplay::resetToNCloumns(int firstColumn, int lastColumn, int row) const {
  const int includedLastColumn = lastColumn + 1;
  const int offset = lastColumn - firstColumn + includedLastColumn;
  if (offset >= 16) {
    return;
  }
  this->lcd.setCursor(firstColumn, row);
  for (int i = firstColumn; i < includedLastColumn ; i++) {
    lcd.print(" ");
  }
  this->lcd.setCursor(firstColumn, row);
}


void ShakerDisplay::resetRow(const int row) const {
  lcd.setCursor(0, row);
  for ( int i = 0; i < 16; i++ ) {
    lcd.print(" ");
  }
  lcd.setCursor(0, row);
}

void ShakerDisplay::resetVariables() {
  this->configTitle[0] = '\0';
  this->lastConfigTitle[0] = '\0';
  this->configValueLabel[0] = '\0';
  this->lastConfigValueLabel[0] = '\0';
  this->rpm = 0.0;
  strcpy(this->time, "00:00:00");
  this->temperature = 0.0;
  this->lastRpm = 0.0;
  strcpy(this->lastTime, "00:00:00");
  this->lastTemperature = 0.0;
  this->configValue = 0.0;
  this->lastConfigValue = 0.0;
}


void ShakerDisplay::run()
{
  switch (this->getState())
  {
  case displayState::CONFIGURING:
    showConfigTitle();
    showConfigValue();
    break;
  case displayState::READY:
    showRpm();
    showTime();
    showTemperature();
    break;
  }
}
