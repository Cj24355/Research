#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#define ONE_WIRE_BUS 2
#define SensorPin A0

//set LCD address
LiquidCrystal_I2C lcd(0x27,16,2);

// Calibration parameters (adjust during calibration)
float calibrationOffset = 0.0;
float calibrationSlope = -3.5;

// Temperature Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
  Serial.println("Ready");
  lcd.setCursor(3,0);
  lcd.print("INITIATING");
  delay(3000);
  lcd.clear();
}

// pH Sensor Function
void phGravitySensor() {
  int buf[10], temp;
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  // Sort readings (bubble sort)
  for (int i = 0; i < 10; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  unsigned long avgValue = 0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;
  lcd.setCursor(4, 0);
  lcd.print("pH");
  lcd.setCursor(6,0);
  lcd.print(":");
  Serial.print("pH: ");
  //
  lcd.setCursor(7, 0);
  lcd.print( phValue, 2);
  Serial.println(phValue, 2);
  digitalWrite(13, HIGH);
  //delay(500);
  digitalWrite(13, LOW);
}

// Temperature Sensor Function
void tempSensor() {
   // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  lcd.setCursor(3, 1);
  lcd.print("Temp");
  lcd.setCursor(7,1);
  lcd.print(":");
  Serial.print("Temperature: ");
  lcd.setCursor(8,1);
  lcd.print(sensors.getTempCByIndex(0));
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");
  
  //delay(500);
}

void loop() {
  tempSensor();
  //delay(1000);
  phGravitySensor();
  delay(3000);
}
