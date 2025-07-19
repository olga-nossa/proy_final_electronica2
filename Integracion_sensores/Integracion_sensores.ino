/*  ESP32  |  IMU (MPU‑6050) + LoRa (SX1278 433 MHz)
    ────────────────────────────────────────────────
    - I²C  : SDA 21, SCL 22  (puedes cambiarlos)
    - LoRa : SS 5, RST 14, DIO0 26
    - Salida: CSV  ax,ay,az,gx,gy,gz,temp
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <LoRa.h>

/* ─── Pines LoRa ─── */
constexpr uint8_t LORA_SS   = 5;
constexpr uint8_t LORA_RST  = 34;
constexpr uint8_t LORA_DIO0 = 35;
constexpr long    LORA_FREQ = 433E6;

/* ─── Objetos ─── */
Adafruit_MPU6050 mpu;

/* ─── Funciones ─── */
bool initMPU() {
  if (!mpu.begin()) return false;
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange        (MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth  (MPU6050_BAND_21_HZ);
  return true;
}

bool initLoRa() {
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  return LoRa.begin(LORA_FREQ);
}

String buildPayload(const sensors_event_t& a,
                    const sensors_event_t& g,
                    const sensors_event_t& t) {
  // ax,ay,az,gx,gy,gz,temp
  char buf[96];
  snprintf(buf, sizeof(buf),
           "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
           a.acceleration.x, a.acceleration.y, a.acceleration.z,
           g.gyro.x,        g.gyro.y,        g.gyro.z,
           t.temperature);
  return String(buf);
}

void sendLoRa(const String& payload) {
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();
}

void setup() {
  Serial.begin(115200);
  if (!initMPU())  { Serial.println("MPU6050 not found"); for (;;) delay(10); }
  if (!initLoRa()) { Serial.println("LoRa init failed");  for (;;) delay(10); }
  Serial.println("System ready");
  delay(100);
}

void loop() {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  String payload = buildPayload(a, g, t);
  sendLoRa(payload);

  Serial.println(payload);
  delay(1000);                           // 1 Hz; ajusta lo que necesites
}

/*  ──────────────── Notas para futuro ────────────────
    - Para añadir sensores crea otra función buildPayload()
      que concatene nuevos valores (ej. pres,hum,etc.).
    - Si el paquete supera ~200 bytes, considera enviar
      binario o varios paquetes.
    - Para recepción usa LoRa.parsePacket() y separa con strtok().
*/
