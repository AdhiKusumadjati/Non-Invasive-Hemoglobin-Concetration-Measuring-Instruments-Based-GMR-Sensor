// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int sensorPin = A0;
//int sensorPin2 = A1;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0); // Sets the cursor to col 0 and row 0
  lcd.print("Bit ADC: "); // Prints Sensor Val: to LCD
  lcd.setCursor(0,1); // Sets the cursor to col 1 and row 0
  lcd.print("V Sensor: "); // Prints Sensor Val: to LCD
  
  pinMode(sensorPin, INPUT);
}

void loop()
{
  int sensorVal = analogRead(sensorPin);
  float Voltage = sensorVal * (5.00 / 1023);
  lcd.setCursor(0,0); // Sets the cursor to col 0 and row 0
  lcd.print(analogRead(sensorVal)); // Prints value on Potpin1 to LCD
  lcd.setCursor(0,1); // Sets the cursor to col 1 and row 0
  lcd.print(analogRead(Voltage)); // Prints value on Potpin1 to LCD
  lcd.clear();
}
