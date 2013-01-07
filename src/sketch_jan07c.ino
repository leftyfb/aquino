// #define aref_voltage 3.3

//TODO make output function that checks DEBUG
int tempPin = 0;    // select the input pin for the potentiometer
float aref_voltage = 5.0;

int statusLed = 10;

int alertLed = 7;
float alertLow = 62;
float alertHigh = 90;


void setup() {
  // analogReference(EXTERNAL);
  pinMode(statusLed, OUTPUT);
  pinMode(alertLed, OUTPUT);
  digitalWrite(alertLed, HIGH);

  Serial.begin(9600);
}

void loop() {
  //TODO make statuLed fade in/out (breathe)
  digitalWrite(statusLed, HIGH);

  // read the value from the sensor:
  int sensorValue = analogRead(tempPin);

  // Serial.print("sensor value: ");
  // Serial.println(sensorValue);

  float voltage = sensorValue * aref_voltage;
  voltage /= 1024.0; 

  // Serial.print("voltage: ");
  // Serial.println(voltage);

  float temperatureC = (voltage - 0.5) * 100 ;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  if (temperatureF > alertHigh || temperatureF < alertLow) {
    digitalWrite(alertLed, HIGH);
  } else {
    digitalWrite(alertLed, LOW);
  }

  Serial.print("tempF: ");
  Serial.println(temperatureF);
  
  delay(1000);
  digitalWrite(statusLed, LOW);
  delay(2000);
  // stop the program for for <sensorValue> milliseconds:
//  delay(sensorValue);                  
}

// vim:ft=arduino
