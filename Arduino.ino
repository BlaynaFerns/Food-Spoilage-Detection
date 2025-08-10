
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <DHT.h> // include DHT sensor library

#define DHTPIN 8
#define LDRPIN A1
#define MQ3PIN A0

LiquidCrystal lcd(10, 9, 5, 4, 3, 2);
DHT dht(DHTPIN, DHT11);

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  int ldr_read = analogRead(LDRPIN);
  int gas_read = analogRead(MQ3PIN);

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LDR-");
  lcd.print(ldr_read);

  lcd.setCursor(0, 1);
  lcd.print("MQ3-");
  lcd.print(gas_read);

  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);

  // Call function to send data to ESP8266
  esp8266(t, h, ldr_read, gas_read);

  delay(5000); // Delay between updates (adjust as needed)
}

void esp8266(float temperature, float humidity, int ldr, int gas) {
  SoftwareSerial espSerial(12, 13); // RX, TX pins for ESP8266
  espSerial.begin(9600);

  // Reset ESP8266
  espSerial.println("AT+RST");
  delay(1000);

  // Connect to WiFi
  espSerial.println("AT+CWJAP=\"your_ssid\",\"your_password\""); // Replace with your WiFi credentials
  delay(2000);

  // Set ESP8266 to client mode
  espSerial.println("AT+CWMODE=1");
  delay(1000);

  // Establish TCP connection to ThingSpeak
  espSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
  delay(2000);

  // Prepare and send HTTP GET request to update ThingSpeak fields
  String httpRequest = "GET /update?api_key=your_api_key";
  httpRequest += "&field1=";
  httpRequest += String(temperature);
  httpRequest += "&field2=";
  httpRequest += String(humidity);
  httpRequest += "&field3=";
  httpRequest += String(ldr);
  httpRequest += "&field4=";
  httpRequest += String(gas);
  httpRequest += "FNO5OAHOE4KA9DOW";

  espSerial.print("AT+CIPSEND=");
  espSerial.println(httpRequest.length());
  delay(500);
  espSerial.print(httpRequest);

  delay(15000); // Delay to allow ThingSpeak update interval
}

