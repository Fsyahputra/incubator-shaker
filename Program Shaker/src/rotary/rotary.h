#ifndef ROTARY_H
#define ROTARY_H

enum class RotaryState
{
  IDLE,
  LEFT,
  RIGHT,
};

enum class RotaryButtonState
{
  PRESSED,
  RELEASED,
};

class RotaryEncoder
{

private:
  int pinA;
  int pinB;
  int buttonPin;

  bool ACurrentState;
  bool BCurrentState;
  bool ALastState = false;
  bool BLastState = false;

  bool currentButtonState = false;
  bool lastButtonState = false;

  unsigned long rotationLastDebounceTime = 0;
  const unsigned long rotationDebounceDelay = 50;

  unsigned long buttonLastDebounceTime = 0;
  const unsigned long buttonDebounceDelay = 50;
  RotaryState state = RotaryState::IDLE;
  RotaryButtonState buttonState = RotaryButtonState::RELEASED;
  void initializePins();
  void setState(RotaryState state);
  void setButtonState(RotaryButtonState buttonState);
  void readRotation();
  void readButton();
  bool getCurrentState(int pin);

public:
  RotaryEncoder(int pinA, int pinB, int buttonPin);
  RotaryState getState();
  RotaryButtonState getButtonState();
  void run();
};

#endif // ROTARY_H