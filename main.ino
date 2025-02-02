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
  //pinMode(13, OUTPUT);
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
  float measure = analogRead(A0);  //Read pin A0
  double voltage = measure*5/1024; //Analog-to-Digital Conversion

 
  // PH_step (Voltage/pH Unit) = (Voltage@PH7-Voltage@PH4)/(PH7 - PH4)
  float pH = 7+((2.70 - voltage)/0.1841); // PH_probe = PH7-((Voltage@PH7-Voltage@probe)/PH_step) //Calibrating
  Serial.print("PH: ");
  Serial.println(pH);
  lcd.setCursor(1, 0);
  lcd.print("pH:");
  lcd.setCursor(4, 0);
  lcd.print(pH);
  if(pH <= 7){
    Serial.println(" - Acid");
    lcd.setCursor(8, 0);
    lcd.print(" - ACID"); // ACID    
  }else{
    Serial.println("- NTRL"); // Neutral
    lcd.setCursor(8, 0);
    lcd.print(" - NTRL"); 
  }
  if(pH >= 8){
    Serial.println("Alkaline"); //ALKA
    lcd.setCursor(8, 0);
    lcd.print(" - ALKA");
  }
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
  delay(1000);
}
