#ifndef SHAKER_UI_H
#define SHAKER_UI_H
#include <rotary/rotary.h>
#include <display/display.h>
#include <countdown/countdown.h>

enum class UIState
{
  CONFIGURING,
  READY,
};

enum class internalUiState
{
  NOT_CONFIGURED,
  CONFIGURE_RPM,
  CONFIGURE_HOURS,
  CONFIGURE_MINUTES,
  CONFIGURE_SECONDS,
  CONFIGURE_TIME,
  CONFIGURE_TEMPERATURE,
  CONFIGURED,
};

class ShakerUI
{

private:
  float rpm = 0;
  float hours = 0;
  float minutes = 0;
  float seconds = 0;
  float temperature = 0;


  float currentRpm = 0;
  float currentTemperature = 0;

  RotaryEncoder &rotaryEncoder;
  ShakerDisplay &lcdDisplay;
  ShakerCountdown &counter;
  RotaryButtonState lastButtonState = RotaryButtonState::RELEASED;
  void setState(UIState state);
  int nthButtonPress = 0;
  UIState state = UIState::CONFIGURING;
  internalUiState internalState = internalUiState::NOT_CONFIGURED;
  void setButtonPress();
  void setInternalState(internalUiState state);
  void handleMultipleButtonPress(int pressCount, internalUiState nextState, RotaryButtonState buttonState);
  internalUiState getInternalState();
  void next();
  void reset();

  static void setMinMaxParam(float &param, float min, float max);

  void configureParam(float &param, RotaryState rotaryState, int increment = 10);
  void handleConfigured() const;
  void handleNotConfigured(RotaryButtonState buttonState);
  void handleConfigured(RotaryButtonState buttonState) ;
  void handleConfigureRPM(RotaryState rotaryState, RotaryButtonState buttonState);
  void handleConfigureHours(RotaryState rotaryState, RotaryButtonState buttonState);
  void handleConfigureMinutes(RotaryState rotaryState, RotaryButtonState buttonState);
  void handleConfigureSeconds(RotaryState rotaryState, RotaryButtonState buttonState);
  void handleConfigureTime(RotaryButtonState);
  void handleConfigureTemperature(RotaryState rotaryState, RotaryButtonState buttonState);

public:
  ShakerUI(RotaryEncoder &rotaryEncoder, ShakerDisplay &lcdDisplay, ShakerCountdown &counter);
  void run();
  void setCurrentRpm(float rpm);
  void setCurrentTemperature(float temperature);
  void init();
  float getRpm();
  UIState getState();
};

#endif // SHAKER_UI_H