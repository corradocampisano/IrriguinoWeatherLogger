


//------------------------------------------------------------------------------------------------------------------
// IRRIGUINO VERSION
#define IRRIGUINO_VERSION "0015"


//------------------------------------------------------------------------------------------------------------------
// IRRIGUINO DEBUG
// means:
//   - attached to a PC via the USB connector:
//     - powered by a USB 3 (900mA provided)
//     - using digital pins 1 and 2 for serial debugging 
#define IRRIGUINO_DEBUG true


/*

  IrriguinoWeather<nnnn>.ino
  
  
  Arduino Uno+Wifi+Relay project
  
  
  
  ------------------------------------------------------------------------------------------------------------------
  ------------------------------------------------------------------------------------------------------------------
  1 - HARDWARE:
  ------------------------------------------------------------------------------------------------------------------
  ------------------------------------------------------------------------------------------------------------------
  
  
  what                   where (DOCs)
  ------------------------------------------------------------------------------------------------------------------
  Arduino Uno            ./DOCs/hardware/Arduino Uno
  Wifi Shield            ./DOCs/hardware/Shields/Wifi
  Relays Shield          ./DOCs/hardware/Shields/Relays
  Ground Moisture        ./DOCs/hardware/Sensors/Ground Moisture
  DHT 11                 ./DOCs/hardware/Sensors/DHT 11
  Ambient Light          ./DOCs/hardware/Sensors/Ambient Light
  Barometric             ./DOCs/hardware/Sensors/Barometric
  
  WATER FLOW             MISSING !!!
  
  
  
  ------------------------------------------------------------------------------------------------------------------
  1.1 - DIGITAL PINs LAYOUT:
  ------------------------------------------------------------------------------------------------------------------
  number  detail    usedBy             whatFor                          notes
  ------------------------------------------------------------------------------------------------------------------
   0      ->RX      native serial      serial rx                        to be used with Serial (at least in dev/debug)
   1      <-TX      native serial      serial tx                        to be used with Serial (at least in dev/debug)
   2      normal    LED                monitoring with led blinks       .
   3      PWM       DHT 11 (pull up)   temp and moist digital sensor    .
   4      normal    Wifi Shield        for SSD handshake                .
   5      PWM       Relays             relay 3                          ?
   6      PWM       Relays             relay 2                          ?
   7      normal    Wifi Shield        handshake ?                      .
   8      normal    Relays             relay 4                          ?
   9      PWM       Relays             relay 1                          ?
  10      PWM       Wifi Shield        ?                                .
  11      PWM       Wifi Shield        ?                                .
  12      normal    Wifi Shield        ?                                .
  13      normal    Wifi Shield        ?                                .
  
  ------------------------------------------------------------------------------------------------------------------
  1.2 - ANALOG PINs LAYOUT:
  ------------------------------------------------------------------------------------------------------------------
  number  detail    usedBy               whatFor
  ------------------------------------------------------------------------------------------------------------------
   0      normal    Ambient Light        sensor
   1      normal    Ground Moisture      1st sensor
   2      normal    Ground Moisture      2nd sensor
   3      normal    Ground Moisture      3rd sensor
   4      normal    Ground Moisture      4th sensor
   5      normal    null                 null
   6      normal    null                 null
  
  
  
  ------------------------------------------------------------------------------------------------------------------
  1.3 - I2C BUS (pins SCL and SDA)
  ------------------------------------------------------------------------------------------------------------------
  
  who            whatFor
  ------------------------------------------------------------------------------------------------------------------
  Barometric     Barometric air pressure sensor
  WindStation    ???
  
  
  
  ------------------------------------------------------------------------------------------------------------------
  ------------------------------------------------------------------------------------------------------------------
  2 - SOFTWARE:
  ------------------------------------------------------------------------------------------------------------------
  ------------------------------------------------------------------------------------------------------------------
  
  
  library           whatFor            where (DOCs)
  ------------------------------------------------------------------------------------------------------------------
  WiFi.h            Wifi Shield        ./DOCs/hardware/Shields/Wifi
   
*/

#include "MonitorLed.h"

#include "SerialLogger.h"

#include "Actuators.h"

#include <dht11.h>
#include "Measurements.h"

#include <WiFi.h>
#include "WifiWebClient.h"




//------------------------------------------------------------------------------------------------------------------
// MONITORING ON A LED - IRRIGUINO parameters
#define MONITOR_LED_DIGITAL_PIN 2
// blinking codes
#define MONITOR_LED_SETUP_BLINKS 6
#define MONITOR_LED_MEASURE_BLINKS 1
#define MONITOR_LED_REPORT_BLINKS 2
#define MONITOR_LED_ACTUATE_BLINKS 3
#define MONITOR_LED_ERROR_BLINKS 12
// blining delay
#define MONITOR_LED_BLINK_DELAY 100

// create object
MonitorLed monitorLed(MONITOR_LED_DIGITAL_PIN, 
  MONITOR_LED_SETUP_BLINKS, MONITOR_LED_MEASURE_BLINKS, 
  MONITOR_LED_REPORT_BLINKS, MONITOR_LED_ACTUATE_BLINKS,
  MONITOR_LED_ERROR_BLINKS, MONITOR_LED_BLINK_DELAY);



//------------------------------------------------------------------------------------------------------------------
// SERIAL LOGGER - IRRIGUINO parameters
#define SERIAL_LOGGER_BAUD 9600
#define SERIAL_LOGGER_MAX_LINE_LENGTH 120

// create object
SerialLogger serialLogger(&monitorLed, 
  SERIAL_LOGGER_MAX_LINE_LENGTH, SERIAL_LOGGER_BAUD);



//------------------------------------------------------------------------------------------------------------------
// ACTUATORS OBJECT - IRRIGUINO parameters
/* 
  report above once fixed !!!
  ------------------------------------------------------------------------------------------------------------------
  1.1 - DIGITAL PINs LAYOUT:
  ------------------------------------------------------------------------------------------------------------------
  
  number  detail    usedBy             whatFor                          notes
  ------------------------------------------------------------------------------------------------------------------
  ...
   5      PWM       Relays             relay 3 <-> moist 3              don't use arduino's 5V for leds on Irriguino Board 0.9-dev
   6      PWM       Relays             relay 2 <-> moist 2              don't use arduino's 5V for leds on Irriguino Board 0.9-dev
  ...
   8      normal    Relays             relay 4 <-> moist 4              don't use arduino's 5V for leds on Irriguino Board 0.9-dev
   9      PWM       Relays             relay 1 <-> moist 1              don't use arduino's 5V for leds on Irriguino Board 0.9-dev
   
   safest pair for (pretty tested on) Irriguino Board 0.9-dev is 5 and 8, with the leds disconnected
*/
#define ACTUATORS_RELAY_DIGITAL_PIN1 9
#define ACTUATORS_RELAY_DIGITAL_PIN2 6
#define ACTUATORS_RELAY_DIGITAL_PIN3 5
#define ACTUATORS_RELAY_DIGITAL_PIN4 8

// create object
Actuators actuators = Actuators(&monitorLed, &serialLogger, 
  ACTUATORS_RELAY_DIGITAL_PIN1, ACTUATORS_RELAY_DIGITAL_PIN2,
  ACTUATORS_RELAY_DIGITAL_PIN3, ACTUATORS_RELAY_DIGITAL_PIN4 );


//------------------------------------------------------------------------------------------------------------------
// MEASUREMENTS OBJECT - IRRIGUINO parameters
#define MEASURES_DHT11_DIGITAL_PIN 3
#define MEASURES_AMBLIGHT_ANALOG_PIN 0

#define MEASURES_SOILMOISTS_ANALOG_PIN1 1
#define MEASURES_SOILMOISTS_ANALOG_PIN2 2
#define MEASURES_SOILMOISTS_ANALOG_PIN3 3
#define MEASURES_SOILMOISTS_ANALOG_PIN4 4

// create object
Measurements measurements = Measurements(&monitorLed, &serialLogger, 
  MEASURES_DHT11_DIGITAL_PIN, MEASURES_AMBLIGHT_ANALOG_PIN,
  MEASURES_SOILMOISTS_ANALOG_PIN1, MEASURES_SOILMOISTS_ANALOG_PIN2,
  MEASURES_SOILMOISTS_ANALOG_PIN3, MEASURES_SOILMOISTS_ANALOG_PIN4 );



//------------------------------------------------------------------------------------------------------------------
// WIFI WEB CLIENT

// IRRIGUINO parameters
#define WIFI_WEB_CLIENT_SSID "TISCALI"
#define WIFI_WEB_CLIENT_PASSWORD "848AA46FE3"

// create object
WifiWebClient wifiWebClient = WifiWebClient(&monitorLed, &serialLogger, &measurements, &actuators);

// wifi status
int wifiStatus = WL_IDLE_STATUS;



//------------------------------------------------------------------------------------------------------------------
// IRRIGUINO INTERNALS
#define IRRIGUINO_SETUP_WAITTIME_SECS 1
#define IRRIGUINO_ACTUATION_INTERVAL_SECS 10



//------------------------------------------------------------------------------------------------------------------
void setup()
{
  // warmup delay
  delay(IRRIGUINO_SETUP_WAITTIME_SECS*1000);
  
  // blink entering setup
  monitorLed.blinkSetup();
    
  // initial timing
  long initTime = millis();
  
  // setup serial logging
  serialLogger.init(IRRIGUINO_DEBUG);
  
  // version
  serialLogger.separatorLarge(); serialLogger.separatorLarge();
  serialLogger.logFF(F("Welcome to Irriguino, version "), F(IRRIGUINO_VERSION));
  serialLogger.separatorLarge(); serialLogger.separatorLarge();
  serialLogger.newLine(); serialLogger.newLine();
  
  
  // monitor led blinking codes
  printBlinkingCodes();
  
  
  // timing
  serialLogger.separatorLarge();
  serialLogger.logFL(F("Initial setup STARTED at "), initTime);
  serialLogger.newLine();
  
  
  // memory usage monitoring
  freeMem();
  
  
  // setup measurements
  measurements.init();
  
  
  // setup actuators (sets pins)
  // instructions for Irriguino Board 0.9-dev:
  // - it works with either true or false, if no LED connected to Relays 3,4 on internal +5V
  // - using true allows to connect the two LED (VRC, VR1 and VR2) to Relays output 3 and 4 (COM3/4, NO3, NO4) ???
  actuators.init(IRRIGUINO_DEBUG);
  
    
  // setup WIFI
  wifiWebClient.init(WIFI_WEB_CLIENT_SSID, WIFI_WEB_CLIENT_PASSWORD);
  
  
  // memory usage monitoring
  freeMem();
  
  
  // logging setup complete
  serialLogger.logFL(F("\nInitial setup COMPLETED at "), millis());
  serialLogger.separatorLarge();
  
  // post setup delay
  delay(IRRIGUINO_SETUP_WAITTIME_SECS*1000);
  
  // blink entering setup
  monitorLed.blinkSetup();
}



//------------------------------------------------------------------------------------------------------------------
void loop()
{  
  serialLogger.newLine();
  
  serialLogger.separatorLarge();
  serialLogger.logFL(F("New loop STARTING at "), millis());
  
  
  // turn off all relays while measuring and reporting
  // some conflicts with relays and wifi...
  // let alone power consumption
  allRelaysOff();
  
  
  // memory usage monitoring
  freeMem();
  
  
  // MEASURE
  // regarding power consumption, you should power up 
  // (at least analog) sensors only when measuring
  measure();
  
  
  // memory usage monitoring
  freeMem();
  
  
  wifiStatus = wifiWebClient.checkAndReconnect();
  if (wifiStatus == WL_CONNECTED)
  {
    
    // REPORTING VIA HTTP on WIFI
    report();
    
    
    // memory usage monitoring
    freeMem();
  }
  else
  {
    serialLogger.logFL(F("\nERROR at "), millis());
    
    serialLogger.logFI(F("\twifiWebClient.checkAndReconnect(): "), wifiStatus);
    serialLogger.logFI(F("\texpected value: "), WL_CONNECTED);
  }
  
  
  // ACTUATE
  actuate();
  
  
  // memory usage monitoring
  freeMem();
  
  
  // let actuation take place
  waitEnough();
  
}



//------------------------------------------------------------------------------------------------------------------
void measure()
{
  serialLogger.newLine();
  
  serialLogger.separatorSmall();
  serialLogger.logFL(F("Starting measurements at "), millis());
  
  measurements.measure();
  
  serialLogger.logFL(F("Done with measurements at "), millis());
  serialLogger.separatorSmall();
}



//------------------------------------------------------------------------------------------------------------------
void actuate()
{
  serialLogger.newLine();
  
  serialLogger.separatorSmall();
  serialLogger.logFL(F("Starting with actuators at "), millis());
    
  
  boolean relay1=false;
  boolean relay2=false;
  boolean relay3=false;
  boolean relay4=false;
  
  float soilMoistEdge = 400;
  serialLogger.logFL(F("\n\t Default SoilMoist EDGE is: "), soilMoistEdge);
  
  // hot and dry (relatively)
  if (measurements.getAirTemp()>30 && measurements.getAirMoist()<60) {
    
    soilMoistEdge = 600;
    serialLogger.logFL(F("\t\t it seems to be hot and dry, OVERRIDING to : "), soilMoistEdge);
  }
  else 
  {
    serialLogger.logF(F("\t\t it seems not so hot and dry, keeping DEFAULT"));
  }
  // cold and wet (relatively)  
  if (measurements.getAirTemp()<10 && measurements.getAirMoist()>10) {
    soilMoistEdge = 200;
    serialLogger.logFL(F("\t\t it seems to be cold and wet, OVERRIDING to : "), soilMoistEdge);
  }
  else 
  {
    serialLogger.logF(F("\t\t it seems not so cold and wet, keeping DEFAULT"));
  }
  
  
  // pair 1
  serialLogger.logFL(F("\t Calculating Relay output upon associate SoilMoist input: "), 1);
  serialLogger.logFL(F("\t\t Measured SoilMoist was: "), measurements.getSoilMoist1());
  if (measurements.getSoilMoist1()<soilMoistEdge) relay1 = true;
  serialLogger.logFL(F("\t\t Calculated Relay value: "), relay1);
  
  
  // pair 2
  serialLogger.logFL(F("\t Calculating Relay output upon associate SoilMoist input: "), 2);
  serialLogger.logFL(F("\t\t Measured SoilMoist was: "), measurements.getSoilMoist2());
  if (measurements.getSoilMoist2()<soilMoistEdge) relay2 = true;
  serialLogger.logFL(F("\t\t Calculated Relay value: "), relay2);
  
  
  // pair 3
  serialLogger.logFL(F("\t Calculating Relay output upon associate SoilMoist input: "), 3);
  serialLogger.logFL(F("\t\t Measured SoilMoist was: "), measurements.getSoilMoist3());
  if (measurements.getSoilMoist3()<soilMoistEdge) relay3 = true;
  serialLogger.logFL(F("\t\t Calculated Relay value: "), relay3);
  
  
  // pair 4
  serialLogger.logFL(F("\t Calculating Relay output upon associate SoilMoist input: "), 4);
  serialLogger.logFL(F("\t\t Measured SoilMoist was: "), measurements.getSoilMoist4());
  if (measurements.getSoilMoist4()<soilMoistEdge) relay4 = true;
  serialLogger.logFL(F("\t\t Calculated Relay value: "), relay4);
  
  
  
  monitorLed.blinkActuate();
  actuators.setRelays(relay1, relay2, relay3, relay4);
  
  serialLogger.logFL(F("Done with actuators at "), millis());
  serialLogger.separatorSmall();
}



//------------------------------------------------------------------------------------------------------------------
void report()
{
  serialLogger.newLine();
    
  serialLogger.separatorSmall();
  serialLogger.logFL(F("Starting reporting at "), millis());
  
  monitorLed.blinkReport();
  
  /*
  // NOT TO BE USED ANYMORE BUT STILL INTERESTING
  
  int i = 10;
  float f = 10.45;
  char c = 'c';
  char const * str = "ciao";
  
  // THIS MAKES A SOFT RESTART on VERSION 0009 !?!
  // see SOFT RESTART folder here ?!?
  //serialLogger.log("\nExample printout for HTTP reporting via WiFi: decimal = %d, float = %f, char = %c, string = '%s'... done!", i, f, c, str, "ciao");
  
  // THIS IS AN EXAMPLE FORMATTING with a function I'M LIKELY NOT USING ANYMORE, since now I have serialLogger.logF*, 
  // which is enough for sending data with the HTTP headers. webinos-arduino does the same if I recall correctly.
  serialLogger.log("\t decimal = %d, float = %f, char = %c, string = '%s'... done!", i, f, c, str, "ciao");  
  
  */
  
  monitorLed.on();
  wifiWebClient.sendRq();
  monitorLed.off();
  
  
  monitorLed.on();
  wifiWebClient.readRs();
  monitorLed.off();
  
  
  serialLogger.logFL(F("Done with reporting at "), millis());
  serialLogger.separatorSmall();
}



//------------------------------------------------------------------------------------------------------------------
void waitEnough()
{
  
  /* con un semplice "long", ottengo questo warning:
        IrriguinoWeather0010.ino: In function ‘void loop()’:
        IrriguinoWeather0010.ino:356:19: warning: comparison between signed and unsigned integer expressions [-Wsign-compare]
     
     verificare la questione dei 49 giorni qui:
     https://www.baldengineer.com/arduino-how-do-you-reset-millis.html
     sembrerebbe un millennium bug su scala ridotta...
         
  */
  unsigned long now = millis();
  unsigned long whenToStop = now + IRRIGUINO_ACTUATION_INTERVAL_SECS*1000;
  
  serialLogger.logFL(F("\nLoop DONE, will now pause for (secs): "), IRRIGUINO_ACTUATION_INTERVAL_SECS);
  serialLogger.logFL(F("\t Current internal time ('millis()'); "), now);
  serialLogger.logFL(F("\t Current loop will be ENDING at "), whenToStop);
  serialLogger.separatorLarge();
  
  
  // WAITING HERE WOULD MAKE THE "ACTUATION TIME" TO "GO BY"
  while (millis()<whenToStop)
  {   
    monitorLed.blinkActuate();
    delay(1000);
  }
}



void allRelaysOff()
{
  monitorLed.blinkActuate();
  actuators.allRelays(false);
}



void printBlinkingCodes() {
  serialLogger.logF(F("Blinking Codes and other info for MonitorLed"));
  
  serialLogger.logFL(F("\t on pin: "), MONITOR_LED_DIGITAL_PIN);
  serialLogger.logFL(F("\t blink delay: "), MONITOR_LED_BLINK_DELAY);
  
  serialLogger.logFL(F("\n\t SETUP_BLINKS = "), MONITOR_LED_SETUP_BLINKS);
  serialLogger.logFL(F("\t MEASURE_BLINKS = "), MONITOR_LED_MEASURE_BLINKS);
  serialLogger.logFL(F("\t REPORT_BLINKS = "), MONITOR_LED_REPORT_BLINKS);
  serialLogger.logFL(F("\t ACTUATE_BLINKS = "), MONITOR_LED_ACTUATE_BLINKS);
  serialLogger.logFL(F("\t ERROR_BLINKS = "), MONITOR_LED_ERROR_BLINKS);
  
  serialLogger.logF(F("\n\tMonitorLed will stay on during WiFi activities."));
  serialLogger.newLine();
}


void freeMem()
{
  //serialLogger.logFL(F("Free Memory (not implemented yet): "), -1);
}


