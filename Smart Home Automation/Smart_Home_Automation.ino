#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RELAY_1_PIN 13
#define RELAY_2_PIN 12
#define HC05_CONTROL_PIN 7

char val;
bool light1Status = false; // Variable to track the status of light 1
bool light2Status = false; // Variable to track the status of light 2

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.backlight();
  lcd.init();
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(HC05_CONTROL_PIN, INPUT);
  pinMode(2, INPUT);  // Pin 2 as INPUT
  pinMode(3, OUTPUT); // Pin 3 as OUTPUT
  // Initialize relays to OFF state
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(13, HIGH); // Initially turn off the first light
  digitalWrite(12, HIGH); // Initially turn off the second light
}

void loop() {
  // Attempt to read Bluetooth commands and control relays
  controlRelaysViaBluetooth();

  // Display temperature information on the LCD
  displayTemperature();
  buzzer();
}


void buzzer()
{
       if (digitalRead(2) == HIGH){
           digitalWrite(3, HIGH); // Turn the LED/Buzz ON
           delay(100); // Wait for 100 milliseconds 
           digitalWrite(3, LOW); // Turn the LED/Buzz OFF
           delay(5); // Wait for 500 milliseconds}
       }
}
void controlRelaysViaBluetooth() {
  if (Serial.available()) {
    val = Serial.read();
    Serial.println(val);

    if (val == '1') {
      light1Status = !light1Status; // Toggle the status of light 1
      digitalWrite(RELAY_1_PIN, light1Status ? LOW : HIGH); // Turn on or off the first light based on status
    } else if (val == '2') {
      light2Status = !light2Status; // Toggle the status of light 2
      digitalWrite(RELAY_2_PIN, light2Status ? LOW : HIGH); // Turn on or off the second light based on status
    }
  }

}

void displayTemperature() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity=");
  lcd.print(dht.readHumidity());
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp=");
  lcd.print(dht.readTemperature());
  lcd.print(" Celsius");

  delay(500);
  lcd.clear();
}
