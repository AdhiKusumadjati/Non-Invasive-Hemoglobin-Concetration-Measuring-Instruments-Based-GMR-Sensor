#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include "SD.h"
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc; // define the Real Time Clock object

// pin definition LCD TFT
#define cs   7
#define dc   9
#define rst  8

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

//Millis constants
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
int interval = 300;

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

// char array to print to the screen
char voltagePrintout[6];
char hemoPrintout[6];

void setupDisplay(){
  TFTscreen.begin();                        // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.background(0, 0, 0);            // clear the screen with a black background
  TFTscreen.stroke(255, 255, 255);          // set the font color to white
  
  TFTscreen.setTextSize(2);                 // set the font size
  TFTscreen.text("Voltage \n ", 0, 0);  // write the text to the top left corner of the screen
  TFTscreen.setTextSize(2);                 // set the font size very large for the loop
  TFTscreen.text("V", 120, 20);
  
  //TFTscreen.setTextSize(2);
  //TFTscreen.text("Hemoglobin \n ", 0, 65);
  //TFTscreen.setTextSize(2);
  //TFTscreen.text("g/dL", 120, 85);
  
  //TFTscreen.setTextSize(4);
}

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
  TFTscreen.setTextSize(2);
  TFTscreen.text("File Name: \n", 0, 65);
  TFTscreen.setTextSize(2);
  TFTscreen.text(filename, 0, 85);

  Wire.begin();  
  if (!rtc.begin()) {
    logfile.println("RTC failed");
    #if ECHO_TO_SERIAL
     Serial.println("RTC failed");
    #endif ECHO_TO_SERIAL
  }
   
  logfile.println("Millis,Time,Voltage");    
  #if ECHO_TO_SERIAL
    Serial.println("Millis,Time,Voltage");
  #endif ECHO_TO_SERIAL  // attempt to write out the header to the file
}

void saveData(float Voltage){
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
   
  delay(10); //awalnya delay = 10
  logfile.print(", ");   
  logfile.println(Voltage);
  //logfile.print(", ");   
  //logfile.println(Hemoglobin);
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
  analogReference(EXTERNAL);
  Serial.begin(9600);
  setupDisplay();
  setupDatalogging();
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  smoothing();
  // calculate the average:
  average = total / numReadings;
  //sensorVal = analogRead(sensorPin);
  float Voltage = (average * (4.07 / 1023)) - 2.6; //0.7
  //sensorVal = map(sensor, -2, 40, 0, 255); //Scale to sensor output to saturation level
  //float Hemoglobin = Voltage * 6.5;

  String strVoltage = String (Voltage);
  //String strHemo = String (Hemoglobin);
  
  // convert the reading to a char array
  strVoltage.toCharArray(voltagePrintout, 6);
  //strHemo.toCharArray(hemoPrintout, 6);

  // set the font color
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.setTextSize(4);
  // print the sensor value
  TFTscreen.text(voltagePrintout, 0, 20);
  //TFTscreen.text(hemoPrintout, 0, 85);
  
  unsigned long currentMillis = millis();

  while (millis() < (currentMillis + interval)){
    smoothing();
    // calculate the average:
    average = total / numReadings;
    //sensorVal = analogRead(sensorPin);
    float Voltage = (average * (4.07 / 1023)) - 2.6;
    //sensorVal = map(sensor, -2, 40, 0, 255); //Scale to sensor output to saturation level
    //float Hemoglobin = Voltage * 20;
    saveData(Voltage);
    #if ECHO_TO_SERIAL
     //Serial.print(", ");   
     //Serial.print(average);
     //Serial.print(", ");   
     Serial.println(Voltage);
     //Serial.print(", ");   
     //Serial.println(Hemoglobin);
    #endif ECHO_TO_SERIAL
    delay(1);
  }

  TFTscreen.stroke(0, 0, 0);
  TFTscreen.setTextSize(4);
  TFTscreen.text(voltagePrintout, 0, 20);
  //TFTscreen.text(hemoPrintout, 0, 85);
}
