#include <math.h>

// TODOs {{{
//TODO make output function that checks DEBUG
//TODO make statuLed fade in/out (breathe)
//TODO switch to calibrated 3.3v for ambient temp sensor
// }}}

// Configuration {{{

// Analog pin number, and input voltage for temperature sensors {{{
int waterTempPin = 0;
int ambientTempPin = 1;
// temp pins }}}

// Digital pin numbers for status and alert LEDs {{{
int statusLed = 10;
int alertLedG = 7;
int alertLedR = 8;
// LEDs }}}

// tempCerature range for alerts {{{
double alertLow = 65;
double alertHigh = 85;
// temp range }}}

// Configuration }}}

double get_temp(int pin, int type) { // {{{
  double tempF;
  double tempC;

  // read the value from the temp sensor:
  float voltage = analogRead(pin);

  switch (type) {

    case 1:
      voltage /= 1024.0; 
      tempC = (voltage * 5.0 - 0.5) * 100 ;
      tempF = (tempC * 9.0 / 5.0) + 32.0;
    break;

    // thermistor
    case 2:
      tempC = log(((10240000/voltage) - 10000));
      tempC = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempC * tempC )) * tempC );
      tempC = tempC - 273.15;            // Convert Kelvin to Celcius
      tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    break;
  }
  
  return tempF;
} // }}}

void setup() {

  //configure LEDs for output
  pinMode(statusLed, OUTPUT);
  pinMode(alertLedG, OUTPUT);
  pinMode(alertLedR, OUTPUT);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // Open serial for output
  Serial.begin(9600);
}

void loop() {

  // Turn on status LED to indivate polling
  digitalWrite(statusLed, HIGH);

  double ambientTemp = get_temp(ambientTempPin, 1);
  double waterTemp = get_temp(waterTempPin, 2);

  //TODO bust temperature bounds checking out into separate function
  if (waterTemp > alertHigh || waterTemp < alertLow) {
    digitalWrite(alertLedR, HIGH);
    digitalWrite(alertLedG, LOW);
  } else {
    digitalWrite(alertLedR, LOW);
    digitalWrite(alertLedG, HIGH);
  }

  if (waterTemp < alertLow) {
    digitalWrite(11, LOW); // set the relay 1 off
    delay(2000); // wait for 2 secs
  } else {
    digitalWrite(11, HIGH); // set the relay 1 on
    delay(1000); // wait for a second
  }

  Serial.print(ambientTemp);
  Serial.print(":");
  Serial.println(waterTemp);
  // Wait a sec before turning off status LED
  delay(1000);
  digitalWrite(statusLed, LOW);

  // Wait additional 2secs before re-checking temp
  delay(2000);
  
//  digitalWrite(12, LOW); // set the relay 2 on
//  delay(1000); // wait for a second
//  digitalWrite(12, HIGH); // set the relay 2 off
//  delay(2000); // wait for 2 secs
}

// vim:ft=arduino
