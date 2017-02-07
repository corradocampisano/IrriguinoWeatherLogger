/*
  WifiWebClient.h - Library for a Wifi Web Client
*/


#ifndef WifiWebClient_h
#define WifiWebClient_h


#include "Arduino.h"

#include "SerialLogger.h"
#include "Measurements.h"
#include "Actuators.h"


class WifiWebClient
{
  public:
  
    WifiWebClient(MonitorLed* monitorLed, SerialLogger * serialLogger, Measurements* measurements, Actuators* actuators);
    
    
    void init(const char * ssid, const char * password);
    
    int checkAndReconnect();
    
    void printWifiStatus();
    
    void sendRq();
    void readRs();
    
    
  private:
  
    MonitorLed* _monitorLed;
    SerialLogger * _serialLogger;
    Measurements* _measurements;
    Actuators* _actuators;
    
    const char * _ssid;
    const char * _password;

    void addHeaderF(String* rqBuffer, const char* name, float value);
    void printWifiConst();
    void buildHttpHeaderBlock(char * buffer, const char * format, ...);
    
};

#endif
