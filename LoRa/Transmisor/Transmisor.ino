// Transmisor
#include <SPI.h>
#include <LoRa.h>

#define SS    5
#define RST   34
#define DIO0  35

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }

  Serial.println("LoRa transmitter ready.");
}

void loop() {
  Serial.println("Sending message...");
  LoRa.beginPacket();
  LoRa.print("Hello from la ESP32 de Olga!");
  LoRa.endPacket();
  
  delay(2000);  // Espera 2 segundos antes de volver a enviar
}
