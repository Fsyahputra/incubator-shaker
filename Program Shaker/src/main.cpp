#include <Arduino.h>
#include <constant/hardware.h>
#include <controller/controller.h>
#include <UI/ui.h>
#include <stepper/stepper.h>
#include <rotary/rotary.h>
#include <LiquidCrystal_I2C.h>


namespace DIR = Hardware::Stepper::Dir;
namespace STEP = Hardware::Stepper::Step;
namespace ROTARY = Hardware::RotaryEncoder;
namespace LCD = Hardware::LcdI2C;

int dirPins[4] = {DIR::X, DIR::Y, DIR::Z, DIR::A};
int stepPins[4] = {STEP::X, STEP::Y, STEP::Z, STEP::A};

LiquidCrystal_I2C lcd(LCD::LCD_ADDR, LCD::LCD_COLS, LCD::LCD_ROWS);
RotaryEncoder re(ROTARY::A, ROTARY::B, ROTARY::BUTTON);
ShakerDisplay lcdDisplay(lcd, LCD::SDA, LCD::SCL);
ShakerUI ui(re, lcdDisplay);
ShakerStepper stepper(stepPins, dirPins);
ShakerController controller(stepper, ui);

void setup()
{
  Serial.begin(Hardware::SerialCfg::BAUD_RATE);
  Serial.println("Shaker Controller Starting...");
  controller.init();
}

void loop()
{
  controller.run();
}