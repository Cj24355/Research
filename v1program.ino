#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
#define SensorPin A0

// Calibration parameters (adjust during calibration)
float calibrationOffset = 0.0;
float calibrationSlope = -3.5;

// Temperature Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
  Serial.println("Ready");
}

// pH Sensor Function
void phGravitySensor() {
  int buf[10], temp;
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(SensorPin);
    delay(10); // Fixed syntax
  }
  // Sort readings (bubble sort)
  for (int i = 0; i < 10; i++) { // Fixed loop limit
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  // Average middle 6 values
  unsigned long avgValue = 0;
  for (int i = 2; i < 8; i++) avgValue += buf[i];
  float voltage = (float)avgValue * 5.0 / 1024 / 6;
  float phValue = 7.0 + ((calibrationOffset - (voltage * 1000)) / calibrationSlope); // Calibrated formula
  Serial.print("pH: ");
  Serial.println(phValue, 2);
  digitalWrite(13, HIGH);
  delay(800);
  digitalWrite(13, LOW);
}

// Temperature Sensor Function
void tempSensor() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Temperature sensor disconnected!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");
}

void loop() {
  tempSensor();
  delay(1000);
  phGravitySensor();
  delay(1000);
}
