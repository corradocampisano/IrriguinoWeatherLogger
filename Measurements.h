/*
  Measurements.h - Library for handling measurements for Irriguino sensors
*/

#ifndef Measurements_h
#define Measurements_h

#include "Arduino.h"
#include "SerialLogger.h"


class Measurements
{
  
  public:
  
    Measurements(
      MonitorLed* monitorLed, SerialLogger* serialLogger, 
      int dhtPin, int ambLightPin,
      int soilMoistPin1, int soilMoistPin2,
      int soilMoistPin3, int soilMoistPin4 );
    
    void init();
    
    int measure();
    
    
    // getters
    float getAirTemp();
    float getAirMoist();
    
    float getSoilMoist1();
    float getSoilMoist2();
    float getSoilMoist3();
    float getSoilMoist4();
    
    float getPressure();
    
    float getAmbLight();
    
    float getWindDir();
    float getWindSpeed();
    float getWindGusts();
    
    float getWaterFlow();
    
    
  private:
    MonitorLed* _monitorLed;
    SerialLogger* _serialLogger;  
    
    void measureAirTempAndMoistFromDht();
    void measureAmbLight();
    void measureSoilMoists();
    
    // pins
    int _dhtPin;
    int _ambLightPin;
    
    int _soilMoistPin1;
    int _soilMoistPin2;
    int _soilMoistPin3;
    int _soilMoistPin4;
    
    // measures
    float _airTemp;
    float _airMoist;
    
    float _soilMoist1;
    float _soilMoist2;
    float _soilMoist3;
    float _soilMoist4;
    
    float _pressure;
    
    float _ambLight;
    
    float _windDir;
    float _windSpeed;
    float _windGusts;
    
    float _waterFlow;

};

#endif
