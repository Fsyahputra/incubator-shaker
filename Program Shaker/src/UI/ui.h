#ifndef SHAKER_UI_H
#define SHAKER_UI_H
#include <rotary/rotary.h>
#include <display/display.h>

enum class UIState
{
  CONFIGURING,
  READY,
};

enum class internalUiState
{
  NOT_CONFIGURED,
  CONFIGURE_RPM,
  CONFIGURE_TIME,
  CONFIGURE_TEMPERATURE,
  CONFIGURED,
};

class ShakerUI
{

private:
  float rpm = 0;
  float time = 0;
  float temperature = 0;

  RotaryEncoder &rotaryEncoder;
  ShakerDisplay &lcdDisplay;
  void setState(UIState state);
  int nthButtonPress = 0;
  UIState state = UIState::CONFIGURING;
  internalUiState internalState = internalUiState::NOT_CONFIGURED;
  void setButtonPress();
  void setInternalState(internalUiState state);
  void handleMultipleButtonPress(int pressCount, internalUiState nextState);
  internalUiState getInternalState();
  void next();
  void reset();

  void setMinMaxParam(float &param, float min, float max);
  void configureParam(float &param, RotaryState rotaryState, int increment = 10);
  void handleConfigureRPM(RotaryState rotaryState, RotaryButtonState buttonState);
  void handleConfigureTime(RotaryState rotaryState, RotaryButtonState buttonState);
  void handleConfigureTemperature(RotaryState rotaryState, RotaryButtonState buttonState);

public:
  ShakerUI(RotaryEncoder &rotaryEncoder, ShakerDisplay &lcdDisplay);
  void run();
  UIState getState();
};

#endif // SHAKER_UI_H