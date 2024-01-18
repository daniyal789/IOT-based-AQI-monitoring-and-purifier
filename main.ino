#include <LiquidCrystal.h>      //Header file for LCD
const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2; //pins of LCD connected to Arduino
LiquidCrystal lcd(rs,en,d4,d5,d6,d7); //lcd function from LiquidCrystal

int buz = 8;  //buzzer connected to pin 8
int led = 9;  //led connected to pin 9
int driver=7;

const int aqsensor = A0;  //output of mq135 connected to A0 pin of Arduino
int threshold = 275;      //Threshold level for Air Quality
int measurePin = A5;
int ledPower = 3;
int samplingTime = 280;
int deltaTime = 40;
int SleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
void setup() {
  pinMode(ledPower, OUTPUT);
  pinMode (buz,OUTPUT);     // buzzer is connected as Output from Arduino
  pinMode (led,OUTPUT);     // led is connected as output from Arduino
  pinMode (aqsensor,INPUT); // MQ135 is connected as INPUT to arduino
  pinMode(driver,OUTPUT);
 

  Serial.begin (9600);      //begin serial communication with baud rate of 9600

  digitalWrite(driver,HIGH);
  lcd.clear();              // clear lcd
  lcd.begin (16,2);         // consider 16,2 lcd
}

void loop() {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(SleepTime);

  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 170 * calcVoltage - 0.1;

  // Print the values to the Serial Monitor
  Serial.print("voMeasured: ");
  Serial.println(voMeasured);
  Serial.print("calcVoltage: ");
  Serial.println(calcVoltage);
  Serial.print("dustDensity: ");
  Serial.println(dustDensity);

  // Add a delay to control the rate of Serial Monitor output
  delay(1000); // Add a 1-second delay between readings (you can adjust this as needed)


  
  int ppm = analogRead(aqsensor); //read MQ135 analog outputs at A0 and store it in ppm

  Serial.print("Air Quality: ");  //print message in serail monitor
  Serial.println(ppm);            //print value of ppm in serial monitor

  lcd.setCursor(0,0);             // set cursor of lcd to 1st row and 1st column
  lcd.print("Air Quality: ");      // print message on lcd
  lcd.print(ppm/2);                 // print value of MQ135
  if ((ppm/2)> threshold)            // check is ppm is greater than threshold or not
    {
      lcd.setCursor(1,1);         //jump here if ppm is greater than threshold
      lcd.print("AQ Level HIGH");
      Serial.println("AQ Level HIGH");     
      tone(led,1000,200);         //blink led with turn on time 1000mS, turn off time 200mS
      digitalWrite(buz,HIGH);     //Turn ON Buzzer
      digitalWrite(driver,HIGH);
    }
  else
    {
      digitalWrite(driver,LOW);
      digitalWrite(led,LOW);   //jump here if ppm is not greater than threshold and turn off LED
      digitalWrite(buz,LOW);   //Turn off Buzzer
      lcd.setCursor(1,1);
      lcd.print ("AQ Level Good");
      Serial.println("AQ Level Good");
    }  
    delay(500);
    delay(3000);
  lcd.clear();
  lcd.print("dust density: ");
  lcd.print(dustDensity);
}


// // other code
// // MQ135 sensor pin configuration
// const int MQ135Pin = A0;

// // Function to calculate Air Quality Index (AQI)
// int calculateAQI(float rs_ro_ratio) {
//   // Adjust these constants based on sensor calibration
//   const float a = 0.18;
//   const float b = -0.38;

//   // Calculate ppm concentration using the ratio and sensor characteristics
//   float ppm = pow(10, (log10(rs_ro_ratio) - b) / a);

//   // AQI calculation formula (adjust coefficients based on specific pollutant)
//   int aqi = 0;
//   if (ppm >= 0.0 && ppm <= 50.0) {
//     aqi = map(ppm, 0, 50, 0, 50);
//   } else if (ppm > 50.0 && ppm <= 200.0) {
//     aqi = map(ppm, 51, 200, 51, 100);
//   } 
//   // Add more conditions for different AQI ranges as needed

//   return aqi;
// }

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   // Read analog value from MQ135 sensor
//   int sensorValue = analogRead(MQ135Pin);

//   // Convert analog value to voltage
//   float voltage = sensorValue * (5.0 / 1023.0);

//   // Calculate the ratio of Rs/Ro
//   float rs_ro_ratio = voltage / (5.0 - voltage);

//   // Calculate AQI
//   int aqi = calculateAQI(rs_ro_ratio);

//   // Display AQI value
//   Serial.print("AQI: ");
//   Serial.println(aqi);

//   // Add a delay between readings
//   delay(1000);
// }