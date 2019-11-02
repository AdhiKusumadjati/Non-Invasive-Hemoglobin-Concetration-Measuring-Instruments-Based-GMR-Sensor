const int numReadings1 = 25;
const int numReadings2 = 10;

int readings1[numReadings1];      // the readings from the analog input
int readIndex1 = 0;              // the index of the current reading
int total1 = 0;                  // the running total
int average1 = 0;                // the average

int readings2[numReadings2];      // the readings from the analog input
int readIndex2 = 0;              // the index of the current reading
int total2 = 0;                  // the running total
int average2 = 0;                

int data = 1023;

float Voltage = 0;
float Voltage1 = 0;
float Voltage2 = 0;

unsigned long t;

int inputPin = A0;

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int thisReading1 = 0; thisReading1 < numReadings1; thisReading1++) {
    readings1[thisReading1] = 0;
  }
  for (int thisReading2 = 0; thisReading2 < numReadings2; thisReading2++) {
    readings2[thisReading2] = 0;
  }
}

void Average1() {
  // subtract the last reading:
  total1 = total1 - readings1[readIndex1];
  // read from the sensor:
  readings1[readIndex1] = analogRead(inputPin);
  // add the reading to the total:
  total1 = total1 + readings1[readIndex1];
  // advance to the next position in the array:
  readIndex1 = readIndex1 + 1;

  // if we're at the end of the array...
  if (readIndex1 >= numReadings1) {
    // ...wrap around to the beginning:
    readIndex1 = 0;
  }

  // calculate the average:
  average1 = total1 / numReadings1;
  // send it to the computer as ASCII digits
  Voltage1 = average1 * (4.10 / 1023);
  delay(1);        // delay in between reads for stability
}

void Average2() {
  // subtract the last reading:
  total2 = total2 - readings2[readIndex2];
  // read from the sensor:
  readings2[readIndex2] = average1;
  // add the reading to the total:
  total2 = total2 + readings2[readIndex2];
  // advance to the next position in the array:
  readIndex2 = readIndex2 + 1;

  // if we're at the end of the array...
  if (readIndex2 >= numReadings2) {
    // ...wrap around to the beginning:
    readIndex2 = 0;
  }

  // calculate the average:
  average2 = total2 / numReadings2;
  // send it to the computer as ASCII digits
  Voltage2 = average2 * (4.10 / 1023);
  delay(1);        // delay in between reads for stability
}

void loop(){
  t = millis();
  Average1();
  Average2();
  /*
  //Serial.print(t);
  Serial.print(", ");
  Voltage = analogRead(inputPin) * (4.10 / 1023);
  Serial.print(Voltage);
  Serial.print(", ");
  delay(1);        // delay in between reads for stability
  Serial.print(Voltage1);
  Serial.print(", ");
  Serial.println(Voltage2);
*/

  //Serial.print(t);
  Serial.print(analogRead(inputPin));
  Serial.print(", ");
  delay(1);        // delay in between reads for stability
  //Serial.print(data);
  Serial.print(", ");
  Serial.print(average1);
  Serial.print(", ");
  Serial.println(average2);

}

