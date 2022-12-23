#include "DHT.h"
#define DHTTYPE DHT11 // DHT 11
 
const int DHTPin = D3;
 
DHT dht(DHTPin, DHTTYPE);     // Initialize DHT sensor.
 
void setup() 
{
  Serial.begin(9600);
  delay(10);
  dht.begin();
}
 
void loop() 
{
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
  }
  else 
  {
    delay(2000);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" % \ t Temperature: ");
    Serial.print(t);
    Serial.print(" *C \ ");
    Serial.println(f);
  }
}
