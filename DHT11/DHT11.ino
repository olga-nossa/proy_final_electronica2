#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 18
#define DHTTYPE DHT11   // o DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    Serial.printf("Temp: %.1f °C  Hum: %.1f %%\n", t, h);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
  delay(500);
}
