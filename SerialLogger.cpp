/*
  SerialLogger.cpp - Library for Serial Logging
*/



#include "Arduino.h"

#include "MonitorLed.h"

#include "SerialLogger.h"

#define LINE_SIZE       80
#define SEPARATOR "----------------------------------"



SerialLogger::SerialLogger(MonitorLed* monitorLed, int lineLen, int baud)
{
  _lineLen=lineLen;
  _baud=baud;
  _monitorLed=monitorLed;
}


void SerialLogger::init(boolean debugMode)
{
  if (debugMode) {
    Serial.begin(_baud);
    
    while (!Serial) {
      // wait for serial port to connect. Needed for native USB
      _monitorLed->blinkSetup();
    }
    this->logF(F("\n\n\n"));
    this->logF(F("SerialLogger initialized\n"));
  }
  else
  {
    _monitorLed->blinkError();
  }
}



void SerialLogger::log(const char * format...) {
  char buffer[LINE_SIZE] = "";

  va_list args;
  va_start(args, format);

  vsprintf (buffer,format, args);

  va_end(args);

  Serial.println(buffer);
}



void SerialLogger::logF(const __FlashStringHelper* stringConstantFromFlash) {
  Serial.println(stringConstantFromFlash);
}



void SerialLogger::logFS(const __FlashStringHelper* stringConstantFromFlash, const char* stringVariable)
{
  Serial.print(stringConstantFromFlash);
  Serial.println(stringVariable);
}
void SerialLogger::logFL(const __FlashStringHelper* stringConstantFromFlash, const long longVariable)
{
  Serial.print(stringConstantFromFlash);
  Serial.println(longVariable);
}
void SerialLogger::logFI(const __FlashStringHelper* stringConstantFromFlash, const int intVariable)
{
  Serial.print(stringConstantFromFlash);
  Serial.println(intVariable);
}

void SerialLogger::logFF(const __FlashStringHelper* stringConstantFromFlash1, const __FlashStringHelper* stringConstantFromFlash2)
{
  Serial.print(stringConstantFromFlash1);
  Serial.println(stringConstantFromFlash2);
}


void SerialLogger::separatorSmall()
{
  Serial.println(SEPARATOR);
}
void SerialLogger::separatorLarge()
{
  Serial.print(SEPARATOR);
  Serial.println(SEPARATOR);
}

void SerialLogger::newLine()
{
  Serial.println();
}    
    
