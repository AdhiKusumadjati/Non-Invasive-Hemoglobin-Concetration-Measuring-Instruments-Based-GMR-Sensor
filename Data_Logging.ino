#include "SD.h"
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC; // define the Real Time Clock object

// A simple data logger for the Arduino analog pins
#define LOG_INTERVAL  500 // mills between entries
#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

// The analog pins that connect to the sensors
#define sensorPin 0           // photocellPin >> analog 0
 
// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;
 
// the logging file
File logfile;
 
void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);
  
  while(1);
}

void setup() {
      Serial.begin(9600);
      Serial.println();
      /*
      #if WAIT_TO_START
        Serial.println("Type any character to start");
        while (!Serial.available());
      #endif //WAIT_TO_START
    */
      // initialize the SD card
      Serial.print("Initializing SD card...");
      // make sure that the default chip select pin is set to
      // output, even if you don't use it:
      pinMode(10, OUTPUT);
      
      // see if the card is present and can be initialized:
      if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
      }
      Serial.println("card initialized.");
      
      // create a new file
      char filename[] = "DATA00.TXT";
      for (uint8_t i = 0; i < 100; i++) {
        filename[4] = i/10 + '0';
        filename[5] = i%10 + '0';
        if (! SD.exists(filename)) {
          // only open a new file if it doesn't exist
          logfile = SD.open(filename, FILE_WRITE); 
          break;  // leave the loop!
        }
      }
      
      if (! logfile) {
        error("couldnt create file");
      }
      
      Serial.print("Logging to: ");
      Serial.println(filename);
    
      Wire.begin();  
      if (!RTC.begin()) {
        logfile.println("RTC failed");
     #if ECHO_TO_SERIAL
         Serial.println("RTC failed");
     #endif  //ECHO_TO_SERIAL
      }
       
      logfile.println("millis,time,light,temp");    
     #if ECHO_TO_SERIAL
      Serial.println("millis,time,light,temp");
     #endif ECHO_TO_SERIAL  // attempt to write out the header to the file
     /* if (logfile.writeError || !logfile.sync()) {
        error("write header");
      }
      */
      pinMode(redLEDpin, OUTPUT);
      pinMode(greenLEDpin, OUTPUT);
}

void loop(void)
{
      // delay for the amount of time we want between readings
      delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
      
      digitalWrite(greenLEDpin, HIGH);
     
      // log milliseconds since starting
      uint32_t m = millis();
      logfile.print(m);           // milliseconds since start
      logfile.print(", ");    
    #if ECHO_TO_SERIAL
      Serial.print(m);         // milliseconds since start
      Serial.print(", ");  
    #endif
     
      // fetch the time
      DateTime now = RTC.now();
      // log time
      //logfile.print(now.get()); // seconds since 2000
      logfile.print(", ");
      logfile.print(now.year(), DEC);
      logfile.print("/");
      logfile.print(now.month(), DEC);
      logfile.print("/");
      logfile.print(now.day(), DEC);
      logfile.print(" ");
      logfile.print(now.hour(), DEC);
      logfile.print(":");
      logfile.print(now.minute(), DEC);
      logfile.print(":");
      logfile.print(now.second(), DEC);
    #if ECHO_TO_SERIAL
      //Serial.print(now.get()); // seconds since 2000
      //Serial.print(", ");
      Serial.print(now.year(), DEC);
      Serial.print("/");
      Serial.print(now.month(), DEC);
      Serial.print("/");
      Serial.print(now.day(), DEC);
      Serial.print(" ");
      Serial.print(now.hour(), DEC);
      Serial.print(":");
      Serial.print(now.minute(), DEC);
      Serial.print(":");
      Serial.print(now.second(), DEC);
    #endif //ECHO_TO_SERIAL
      int sensorReading = analogRead(sensorPin);  
      delay(10);
      logfile.print(", ");    
      logfile.println(sensorReading);
      logfile.flush();
      //logfile.close();
    #if ECHO_TO_SERIAL
      Serial.print(", ");   
      Serial.println(sensorReading);
    #endif //ECHO_TO_SERIAL
      digitalWrite(greenLEDpin, LOW);
}
