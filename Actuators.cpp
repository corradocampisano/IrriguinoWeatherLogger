/*
  Actuators.h - Library for handling actuators for Irriguino (relays)
*/



#include "Arduino.h"
#include "Actuators.h"



Actuators::Actuators(
  MonitorLed* monitorLed, SerialLogger* serialLogger, 
  int relayPin1, int relayPin2,
  int relayPin3, int relayPin4 )
{
  _monitorLed=monitorLed;
  _serialLogger=serialLogger;
  
  _relayPin1=relayPin1;
  _relayPin2=relayPin2;
  _relayPin3=relayPin3;
  _relayPin4=relayPin4;
}





void Actuators::init(boolean debugMode)
{
  if (debugMode) {
    setupDevBoardPair();
  }
  else
  {
    setupProdBoard();
  }
}



void Actuators::allRelays(boolean on)
{
  _serialLogger->logFL(F("\nSetting all (valid) Relays to: "), on);
  
  storePrevs();
  
  _relayValue1=on;
  _relayValue2=on;
  _relayValue3=on;
  _relayValue4=on;
  
  if (_validPin1) {
    logChanges(_relayPin1, _relayValue1);
    digitalWrite(_relayPin1, _relayValue1);
    delay(100);
  }
  
  if (_validPin2) {
    logChanges(_relayPin2, _relayValue2);
    digitalWrite(_relayPin2, _relayValue2);
    delay(100);
  }
  
  if (_validPin3) {
    logChanges(_relayPin3, _relayValue3);
    digitalWrite(_relayPin3, _relayValue3);
    delay(100);
  }
  
  if (_validPin4) {
    logChanges(_relayPin4, _relayValue4);
    digitalWrite(_relayPin4, _relayValue4);
    delay(100);
  }
}



void Actuators::setRelays(boolean relay1, boolean relay2, boolean relay3, boolean relay4)
{
  _serialLogger->logF(F("\nActuating outputs on all (valid) Relays (on=1, off=0)"));
  
  storePrevs();
  
  _relayValue1=relay1;
  _relayValue2=relay2;
  _relayValue3=relay3;
  _relayValue4=relay4;
  
  if (_validPin1) {
    logChanges(_relayPin1, _relayValue1);
    digitalWrite(_relayPin1, _relayValue1);
    delay(100);
  }
  
  if (_validPin2) {
    logChanges(_relayPin2, _relayValue2);
    digitalWrite(_relayPin2, _relayValue2);
    delay(100);
  }
  
  if (_validPin3) {
    logChanges(_relayPin3, _relayValue3);
    digitalWrite(_relayPin3, _relayValue3);
    delay(100);
  }
  
  if (_validPin4) {
    logChanges(_relayPin4, _relayValue4);
    digitalWrite(_relayPin4, _relayValue4);
    delay(100);
  }
}
      


void Actuators::logChanges(int pin, boolean value)
{
  _serialLogger->logFL(F("\t relay on pin: "), pin);
  _serialLogger->logFL(F("\t will be set to: "), value);
}



void Actuators::storePrevs()
{
  _serialLogger->logFL(F("\t storing previous values for all relays at "), millis());
  
  _relayPrevValue1=_relayValue1;
  _relayPrevValue2=_relayValue2;
  _relayPrevValue3=_relayValue3;
  _relayPrevValue4=_relayValue4;
}



// GETTERS


boolean Actuators::getRelayValue1()
{
  return _relayPrevValue1;
}
boolean Actuators::getRelayValue2()
{
  return _relayPrevValue2;
}
boolean Actuators::getRelayValue3()
{
  return _relayPrevValue3;
}
boolean Actuators::getRelayValue4()
{
  return _relayPrevValue4;
}







//------------------------------------------------------------------------------------------------------------------
// SETTING UP AND TESTING PINS FOR RELAYS
//------------------------------------------------------------------------------------------------------------------

/*
  number  detail    usedBy             whatFor                          notes
  ------------------------------------------------------------------------------------------------------------------
  ...
   5      PWM       Relays             relay 3                          ok
   6      PWM       Relays             relay 2                          don't use its 5V for leds
  ...
   8      normal    Relays             relay 4                          ok
   9      PWM       Relays             relay 1                          don't use its 5V for leds
*/
void Actuators::setupDevBoardPair()
{
  setupTestAllPins(false, false, true, true);
}
void Actuators::setupProdBoard()
{
  setupTestAllPins(true, true, true, true);
}



void Actuators::setupTestAllPins(boolean validPin1, boolean validPin2, boolean validPin3, boolean validPin4)
{
  _serialLogger->logF(F("\nSetting up and testing all (valid) Relay Pins (digital outputs)"));
  
  if (validPin1) 
  {
    _serialLogger->logFL(F("\t relayPin1: "), _relayPin1);
    _validPin1=true;
    pinMode(_relayPin1, OUTPUT);
    delay(100);
    digitalWrite(_relayPin1, HIGH);
    delay(100);
    digitalWrite(_relayPin1, LOW);
    delay(100);
  }
  
  if (validPin2) 
  {
    _serialLogger->logFL(F("\t relayPin2: "), _relayPin2);
    _validPin2=true;
    pinMode(_relayPin2, OUTPUT);
    delay(100);
    digitalWrite(_relayPin2, HIGH);
    delay(100);
    digitalWrite(_relayPin2, LOW);
    delay(100);
  }
  
  if (validPin3) 
  {
    _serialLogger->logFL(F("\t relayPin3: "), _relayPin3);
    _validPin3=true;
    pinMode(_relayPin3, OUTPUT);
    delay(100);
    digitalWrite(_relayPin3, HIGH);
    delay(100);
    digitalWrite(_relayPin3, LOW);
    delay(100);
  }
  
  if (validPin4) 
  {
    _serialLogger->logFL(F("\t relayPin4: "), _relayPin4);
    _validPin4=true;
    pinMode(_relayPin4, OUTPUT);
    delay(100);
    digitalWrite(_relayPin4, HIGH);
    delay(100);
    digitalWrite(_relayPin4, LOW);
    delay(100);
  }
}

