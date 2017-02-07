/*
  MonitorLed.h - Library for flashing Monitor Led
*/

#ifndef MonitorLed_h
#define MonitorLed_h


#include "Arduino.h"


class MonitorLed
{
  
  public:
  
    MonitorLed(int pin, 
      int setupBlinks, int measureBlinks, 
      int reportBlinks, int actuateBlinks, 
      int errorBlinks, int delayTime);
    
    
    void on();
    void off();
    
    void blinkSetup();
    
    void blinkMeasure();
    void blinkReport();
    void blinkActuate();
    
    void blinkError();
    
    
  private:
    
    int _pin;
    
    int _setupBlinks;
    
    int _measureBlinks;
    int _reportBlinks;
    int _actuateBlinks;
    
    int _errorBlinks;
    
    int _delayTime;
    
    void blink(int times, int delayTime);
    
};

#endif
