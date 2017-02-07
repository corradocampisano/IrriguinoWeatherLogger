/*
  SerialLogger.h - Library for Serial Logging
*/


#ifndef SerialLogger_h
#define SerialLogger_h


#include "Arduino.h"

#include "MonitorLed.h"



class SerialLogger
{
  public:
    SerialLogger(MonitorLed* monitorLed, int lineLen, int baud);
    
    void init(boolean debugMode);
    
    void log(const char *format...);
    
    void logF(const __FlashStringHelper* stringConstantFromFlash);
    
    void logFS(const __FlashStringHelper* stringConstantFromFlash, const char* stringVariable);
    void logFL(const __FlashStringHelper* stringConstantFromFlash, const long longVariable);
    void logFI(const __FlashStringHelper* stringConstantFromFlash, const int intVariable);
    
    void logFF(const __FlashStringHelper* stringConstantFromFlash1, const __FlashStringHelper* stringConstantFromFlash2);
    
    void separatorSmall();
    void separatorLarge();
    
    void newLine();
    
  private:
    int _lineLen;
    int _baud;
    MonitorLed* _monitorLed;
};

#endif
