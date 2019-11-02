/*long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}*/

int sensorPin = 0; //Define a variable for the sensor output
int sensorVal = 0;
float sensorVoltage;
double Vcc;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //Vcc = readVcc()/1000.0;
  sensorVal = analogRead(0)- analogRead(1); //Read the sensor differential output voltage, out(+) on IO0 and out(-) on IO1 
  //sensorVal = analogRead(sensorPin);
  //sensor = map(sensor, 4, 70, 0, 255); //Scale to sensor output to saturation level
  sensorVoltage = sensorVal * ( 5.0 / 1023.0 );
  // subtract the last reading:
  
  //analogWrite(3, sensor); //Output the sensor level; to IO3
  //Serial.print("GMR ADC = ");
  //Serial.print(sensorVal);
  //Serial.print("    ");
  //Serial.print("Vcc = ");
  //Serial.print(Vcc);
  //Serial.print("    ");
  Serial.print("V GMR = ");
  Serial.print(sensorVoltage);
  Serial.println(" ");
  delay(1);        // delay in between reads for stability
}
