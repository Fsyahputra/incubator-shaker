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
  this->state = newState;
}

void RotaryEncoder::setButtonState(RotaryButtonState newButtonState)
{
  this->buttonState = newButtonState;
}

bool RotaryEncoder::getCurrentState(int pin)
{
  bool currentState = digitalRead(pin);
  return currentState;
}

RotaryState RotaryEncoder::getState()
{
  return this->state;
}

RotaryButtonState RotaryEncoder:: getButtonState()
{
  return this->buttonState;
}

void RotaryEncoder::readRotation()
{
  unsigned long currentTime = millis();



  this->ACurrentState = this->getCurrentState(this->pinA);
  if (this->ACurrentState == this->ALastState )
  {
    setState(RotaryState::IDLE);
    return;
  }

  if (this->ACurrentState != this->ALastState && (currentTime - this->rotationLastDebounceTime) > this->rotationDebounceDelay)
  {

    this->BCurrentState = this->getCurrentState(this->pinB);

    if (this->BCurrentState != this->ACurrentState)
    {
      Serial.println("RIGHT");
      setState(RotaryState::RIGHT);
    }
    else
    {
      Serial.println("LEFT");
      setState(RotaryState::LEFT);
    }

    this->rotationLastDebounceTime = currentTime;
  }

  this->ALastState = this->ACurrentState;
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
      Serial.println("PRESSED");
      setButtonState(RotaryButtonState::PRESSED);
    }
    else
    {
      Serial.println("RELEASED");
      setButtonState(RotaryButtonState::RELEASED);
    }
  } else {
    setButtonState(RotaryButtonState::RELEASED);
  }
  this->lastButtonState = this->currentButtonState;
}

void RotaryEncoder::run()
{
  readRotation();
  readButton();
}