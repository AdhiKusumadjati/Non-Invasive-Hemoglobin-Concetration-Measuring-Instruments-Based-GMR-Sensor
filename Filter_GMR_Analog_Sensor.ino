int sensorPin = 0; //Define a variable for the sensor output
int sensorVal = 0;
float sensorVoltage = 0;

//Averaging
const int numReadings = 20;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup() {
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(sensorPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  sensorVoltage = (average * ( 5.0 / 1023)) - 0.7;
  
  //Serial.print("GMR ADC = ");
  //Serial.print(average);
  //Serial.print("    ");
  //Serial.print("Sensor Voltage: ");
  Serial.println(sensorVoltage);
  delay(1);        // delay in between reads for stability
}
