#include <math.h>
// #define aref_voltage 3.3
//TODO make output function that checks DEBUG


// Analog pin number, and input voltage for temperature sensor
int tempPin = 1;
float aref_voltage = 5.0;

// Digital pin numbers for status and alert LEDs
int statusLed = 10;
int alertLed = 7;



// Temperature range for alerts
float alertLow = 62;
float alertHigh = 90;


double Thermister(int RawADC) {
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 return Temp;
}

// ------thermistor


void setup() {
  //TODO switch to calibrated 3.3v for temp sensor

  //// for use with calibrated analog (3.3v) power
  // analogReference(EXTERNAL);

  //configure LEDs for output
  pinMode(statusLed, OUTPUT);
  pinMode(alertLed, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // Open serial for output
  Serial.begin(9600);
}

void loop() {
  //TODO make statuLed fade in/out (breathe)

  // Turn on status LED to indivate polling
  digitalWrite(statusLed, HIGH);

  // read the value from the temp sensor:
  int sensorValue = analogRead(tempPin);

  // Serial.print("sensor value: ");
  // Serial.println(sensorValue);

  // Calculate mV from temp sensor output
  float voltage = sensorValue * aref_voltage;
  voltage /= 1024.0; 

  // Serial.print("voltage: ");
  // Serial.println(voltage);

  // Convert mV to degrees Celcius and Fahrenheit
  float temperatureC = (voltage - 0.5) * 100 ;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  //TODO bust temperature bounds checking out into separate function
  if (temperatureF > alertHigh || temperatureF < alertLow) {
    digitalWrite(alertLed, HIGH);
  } else {
    digitalWrite(alertLed, LOW);
  }

  // Output temperature to serial port
  // Serial.print("Ambient: ");
  Serial.print(temperatureF);
  Serial.print(":");
  // Serial.print("Water: ");
  Serial.println(int(Thermister(analogRead(0))));  // display Fahrenheit 

  // Wait a sec before turning off status LED
  delay(1000);
  digitalWrite(statusLed, LOW);

  // Wait additional 2secs before re-checking temp
  delay(2000);
  
  digitalWrite(11, LOW); // set the relay 1 on
  delay(1000); // wait for a second
  digitalWrite(11, HIGH); // set the relay 1 off
  delay(2000); // wait for 2 secs
  digitalWrite(12, LOW); // set the relay 2 on
  delay(1000); // wait for a second
  digitalWrite(12, HIGH); // set the relay 2 off
  delay(2000); // wait for 2 secs
}


// vim:ft=arduino
