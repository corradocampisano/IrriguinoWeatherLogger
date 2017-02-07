/*
  MonitorLed.cpp - Library for flashing Monitor Led
*/



#include "Arduino.h"
#include "MonitorLed.h"



MonitorLed::MonitorLed(int pin, 
  int setupBlinks, int measureBlinks, 
  int reportBlinks, int actuateBlinks, 
  int errorBlinks, int delayTime)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  
  _setupBlinks=setupBlinks;
  
  _measureBlinks=measureBlinks;
  _reportBlinks=reportBlinks;
  _actuateBlinks=actuateBlinks;
  
  _errorBlinks=errorBlinks;
  
  _delayTime=delayTime;
}


void MonitorLed::blinkSetup()
{
  blink(_setupBlinks, _delayTime);
}



void MonitorLed::blinkMeasure()
{
  blink(_measureBlinks, _delayTime);
}
void MonitorLed::blinkReport()
{
  blink(_reportBlinks, _delayTime);
}
void MonitorLed::blinkActuate()
{
  blink(_actuateBlinks, _delayTime);
}

void MonitorLed::blinkError()
{
  blink(_errorBlinks, _delayTime);
}



void MonitorLed::on()
{
  digitalWrite(_pin, HIGH);
}

void MonitorLed::off()
{
  digitalWrite(_pin, LOW);
}



void MonitorLed::blink(int times, int delayTime)
{
  for (int i=0; i<times; i++) {
    digitalWrite(_pin, HIGH);
    delay(delayTime);
    digitalWrite(_pin, LOW);
    delay(delayTime);
  }
}

