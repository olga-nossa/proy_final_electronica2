#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial SerialGPS(2);          // UART2

constexpr uint8_t GPS_RX_PIN = 16;    // RX2 ← TX GPS
constexpr uint8_t GPS_TX_PIN = 17;    // TX2 → RX GPS (opcional)

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("Esperando datos del GPS…");
}

void loop() {
  while (SerialGPS.available()) gps.encode(SerialGPS.read());

  if (gps.location.isUpdated()) {
    Serial.printf(
      "\nLat: %.6f  Lon: %.6f  Sat: %u\n",
      gps.location.lat(), gps.location.lng(), gps.satellites.value()
    );

    /* ── Fecha y hora ── */
    if (gps.date.isValid() && gps.time.isValid()) {
      Serial.printf(
        "Fecha (UTC): %02u/%02u/%04u  Hora (UTC): %02u:%02u:%02u\n",
        gps.date.day(), gps.date.month(), gps.date.year(),
        gps.time.hour(), gps.time.minute(), gps.time.second()
      );
    } else {
      Serial.println("Fecha/hora aún no disponible");
    }
  }
}
