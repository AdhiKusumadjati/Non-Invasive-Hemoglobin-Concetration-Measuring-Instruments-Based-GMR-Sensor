#include "SD.h"
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc; // define the Real Time Clock object

// A simple data logger for the Arduino analog pins
#define LOG_INTERVAL  1 // milis between entries
#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// The analog pins that connect to the sensors
const int sensorPin = 0;
int sensorVal = 0;

//Smoothing Constant or averaging
const int numReadings = 25;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

void setupDatalogging(){
  Serial.println();
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
  
  Serial.print("Logging to: ");
  Serial.println(filename);

  Wire.begin();  
  if (!rtc.begin()) {
    logfile.println("RTC failed");
    #if ECHO_TO_SERIAL
     Serial.println("RTC failed");
    #endif ECHO_TO_SERIAL
  }
   
  logfile.println("Millis,Time,Voltage,Hemoglobin");    
  #if ECHO_TO_SERIAL
    Serial.println("Millis,Time,Bit ADC,Voltage,Hemoglobin");
  #endif ECHO_TO_SERIAL  // attempt to write out the header to the file
}

void smoothing() {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(sensorVal);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
}

void setup() {
  //analogReference(EXTERNAL);
  Serial.begin(9600);
  setupDatalogging();
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
 
  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m);           // milliseconds since start
  logfile.print(", ");    
  #if ECHO_TO_SERIAL
    //Serial.print(m);         // milliseconds since start
    //Serial.print(", ");
  #endif ECHO_TO_SERIAL
  
  // Fetch the time
  DateTime now = rtc.now();
  // Log time
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
    //Serial.print(now.year(), DEC);
    //Serial.print("/");
    //Serial.print(now.month(), DEC);
    //Serial.print("/");
    //Serial.print(now.day(), DEC);
    //Serial.print(" ");
    //Serial.print(now.hour(), DEC);
    //Serial.print(":");
    //Serial.print(now.minute(), DEC);
    //Serial.print(":");
    //Serial.print(now.second(), DEC);
  #endif ECHO_TO_SERIAL
  
  smoothing();
  // calculate the average:
  average = total / numReadings;
  //sensorVal = analogRead(sensorPin);
  float Voltage = (average * (4.07 / 1023)) - 2.6; //0.7
  //sensorVal = map(sensor, -2, 40, 0, 255); //Scale to sensor output to saturation level
  float Hemoglobin = Voltage * 6.5;
  delay(10); //awalnya delay = 10
  
  logfile.print(", ");   
  logfile.print(Voltage);
  logfile.print(", ");   
  logfile.println(Hemoglobin);
  logfile.flush();
  #if ECHO_TO_SERIAL
   //Serial.print(", ");   
   //Serial.print(sensorVal);
   //Serial.print(", ");   
   Serial.println(Voltage);
   //Serial.print(", ");   
   //Serial.println(Hemoglobin);
  #endif ECHO_TO_SERIAL
}
