/*This example sketch shows how to change the reference for Arduino's ADC to default or internal 1.1V.
This sketch was used to show the measurement resolution advantage of scaling Arduino's ADC reference to 
the voltage range or scale being measured. This was shown in an ADC tutorial on the ForceTronics YouTube Channel.
This code is free and open for anybody to use at their own risk. 1/6/15
*/

int sensor = 0; //Define a variable for the sensor output
float sensorVoltage;
float Vcc;

float const aVCC = 5.0158; //Actual measured voltage value at AVCC pin. This is im
float const iREF = 1.0644; //Actual measured voltage value of the internal 1.1V reference
//Note that these values can be measured at the AREF pin

void setup() {
  Serial.begin(9600); //setup serial connection
}

void loop() {
SensorRead();
  
  delay(1000); //delay 1 second between each ADC measurement
  analogReference(DEFAULT); //set the ADC reference to default which is AVCC (uses power supply voltage or VCC as reference)
  burn8Readings(); //make 8 readings but don't use them to ensure good reading after reference change
  Serial.println("Sensor ADC Bit Output+ :");
  Serial.println(analogRead(0));
  Serial.println("Sensor ADC Bit Output- :");
  Serial.println(analogRead(1));
  Serial.println("Sensor ADC Bit Output:");
  Serial.println(sensor);
  Serial.println("Default (AVCC) Ref Voltage:");
  Serial.println(convertToVolt(Vcc,sensor),4); //Make ADC measurement at A0, convert it to a voltage value, write value to serial with 4 decimal places 
  Serial.println(" ");
}

long readVcc() {
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
}

//This function makes 8 ADC measurements but does nothing with them
//Since after a reference change the ADC can return bad readings at first. This function is used to get rid of the first 
//8 readings to ensure an accurate one is displayed
void burn8Readings() {
  for(int i=0; i<8; i++) {
    analogRead(A0);
    delay(1);
  }
}

void SensorRead(){
  sensor = analogRead(0) - analogRead(1); //Read the sensor differential output voltage, out(+) on IO0 and out(-) on IO1 
  sensor = map(sensor, 0, 68, 0, 255); //Scale sensor output saturation level to ADC output (0-5 V)
  analogWrite(3, sensor); //Output the sensor level; to IO3
}

//This function convert the ADC level integer value into a useful voltage value.
//The inputs are the measured ADC value and the ADC reference voltage level
//The formula used was obtained from the data sheet: (ADC value / 1024) x ref voltage
float convertToVolt(float refVal, int aVAL) {
  Vcc = readVcc()/1000.0;
  return (((float)aVAL/1024)*Vcc);
}
