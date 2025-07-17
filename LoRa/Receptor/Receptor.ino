// Receptor
#include <SPI.h>
#include <LoRa.h>

#define SS    5
#define RST   14
#define DIO0  26

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }

  Serial.println("LoRa receiver ready.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: ");
    while (LoRa.available()) {
      String message = LoRa.readString();
      Serial.print(message);
    }

    Serial.print(" | RSSI: ");
    Serial.println(LoRa.packetRssi());
  }
}