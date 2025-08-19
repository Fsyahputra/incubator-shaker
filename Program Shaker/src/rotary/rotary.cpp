#include <Arduino.h>
#include <rotary/rotary.h>

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int buttonPin)
    : pinA(pinA), pinB(pinB), buttonPin(buttonPin)
{
  this->pinA = pinA;
  this->pinB = pinB;
  this->buttonPin = buttonPin;
}

void RotaryEncoder::initializePins()
{
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
}

void RotaryEncoder::setState(RotaryState newState)
{
  state = newState;
}

void RotaryEncoder::setButtonState(RotaryButtonState newButtonState)
{
  buttonState = newButtonState;
}

bool RotaryEncoder::getCurrentState(int pin)
{
  bool currentState = digitalRead(pin);
  return currentState;
}

RotaryState RotaryEncoder::getState()
{
  return state;
}

RotaryButtonState RotaryEncoder::getButtonState()
{
  return buttonState;
}

void RotaryEncoder::readRotation()
{
  unsigned long currentTime = millis();
  this->ACurrentState = this->getCurrentState(this->pinA);
  this->BCurrentState = this->getCurrentState(this->pinB);

  if (this->ACurrentState == this->ALastState && this->BCurrentState == this->BLastState)
  {
    setState(RotaryState::IDLE);
    return; // No change
  }

  if (this->ACurrentState != this->ALastState && (currentTime - this->rotationLastDebounceTime) > this->rotationDebounceDelay)
  {
    if (this->BCurrentState != this->ACurrentState)
    {
      setState(RotaryState::RIGHT);
    }
    else
    {
      setState(RotaryState::LEFT);
    }

    this->rotationLastDebounceTime = currentTime;
  }

  this->ALastState = this->ACurrentState;
  this->BLastState = this->BCurrentState;
}

void RotaryEncoder::readButton()
{
  unsigned long currentTime = millis();
  this->currentButtonState = getCurrentState(buttonPin);
  if (this->currentButtonState != this->lastButtonState && (currentTime - this->buttonLastDebounceTime) > this->buttonDebounceDelay)
  {
    this->buttonLastDebounceTime = currentTime;

    if (this->currentButtonState == LOW)
    {
      setButtonState(RotaryButtonState::PRESSED);
    }
    else
    {
      setButtonState(RotaryButtonState::RELEASED);
    }
  }
  this->lastButtonState = this->currentButtonState;
}

void RotaryEncoder::run()
{
  readRotation();
  readButton();
}