/*
  Actuators.h - Library for handling actuators for Irriguino (relays)
*/

#ifndef Actuators_h
#define Actuators_h

#include "Arduino.h"
#include "SerialLogger.h"


class Actuators
{
  
  public:
  
    Actuators(
      MonitorLed* monitorLed, SerialLogger* serialLogger, 
      int relayPin1, int relayPin2,
      int relayPin3, int relayPin4 );
    
    void init(boolean debugMode);
    
    void setRelays(boolean relay1, boolean relay2, 
      boolean relay3, boolean relay4);
      
    void allRelays(boolean on);
    
    boolean getRelayValue1();
    boolean getRelayValue2();
    boolean getRelayValue3();
    boolean getRelayValue4();
    
    
  private:
    MonitorLed* _monitorLed;
    SerialLogger* _serialLogger;  
    
    int _relayPin1;
    int _relayPin2;
    int _relayPin3;
    int _relayPin4;
    
    boolean _validPin1;
    boolean _validPin2;
    boolean _validPin3;
    boolean _validPin4;
    
    boolean _relayValue1;
    boolean _relayValue2;
    boolean _relayValue3;
    boolean _relayValue4;
    
    boolean _relayPrevValue1;
    boolean _relayPrevValue2;
    boolean _relayPrevValue3;
    boolean _relayPrevValue4;
    
    void storePrevs();
    void logChanges(int pin, boolean value);
    
    void setupDevBoardPair();
    void setupProdBoard();
    
    void setupTestAllPins(boolean validPin1, boolean validPin2, boolean validPin3, boolean validPin4);
};

#endif
