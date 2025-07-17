#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);  // UART2

#define GPS_RX_PIN 19  // Conectar al TX del GPS
#define GPS_TX_PIN 20  // Conectar al RX del GPS

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("Esperando datos del GPS...");
}

void loop() {
  while (SerialGPS.available()) {
    char c = SerialGPS.read();
    gps.encode(c);

    // Muestra los datos crudos del GPS (opcional para debug)
    Serial.write(c);
  }

  if (gps.location.isUpdated()) {
    Serial.println("\n=== DATOS GPS ===");
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());
    Serial.println("==================");
  }
}