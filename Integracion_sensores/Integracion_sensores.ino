/*  ESP32 DevKit v1
    GPS + IMU (MPU‑6050) + DHT11 + LoRa SX1278 433 MHz
    Orden CSV: yyyy‑mm‑dd,hh:mm:ss,lat,lon,sat,ax,ay,az,gx,gy,gz,imuT,dhtT,dhtH
*/

/* ── Librerías ── */
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <DHT.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

/* ── Pines ── */
constexpr uint8_t I2C_SDA = 21, I2C_SCL = 22;

constexpr uint8_t LORA_SS = 5, LORA_RST = 34, LORA_DIO0 = 35;
constexpr long    LORA_FREQ = 433E6;

constexpr uint8_t DHTPIN = 4;
constexpr uint8_t DHTTYPE = DHT11;

constexpr uint8_t GPS_RX_PIN = 16;   // RX2 ← TX GPS
constexpr uint8_t GPS_TX_PIN = 17;   // TX2 → RX GPS

/* ── Objetos ── */
Adafruit_MPU6050 mpu;
DHT              dht(DHTPIN, DHTTYPE);
TinyGPSPlus      gps;
HardwareSerial   SerialGPS(2);

/* ── Setup ── */
void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL, 400000);

  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange        (MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth  (MPU6050_BAND_21_HZ);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  LoRa.begin(LORA_FREQ);

  dht.begin();
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("System ready");
}

/* ── Loop ── */
void loop() {
  /* Actualizar GPS */
  while (SerialGPS.available()) gps.encode(SerialGPS.read());

  /* Cada segundo */
  static uint32_t t0 = 0;
  if (millis() - t0 < 1000) return;
  t0 = millis();

  /* Lecturas IMU */
  sensors_event_t acc, gyr, imuT;
  mpu.getEvent(&acc, &gyr, &imuT);

  /* Lecturas DHT */
  float dhtH = dht.readHumidity();
  float dhtT = dht.readTemperature();
  if (isnan(dhtH) || isnan(dhtT)) { dhtH = dhtT = NAN; }

  /* GPS (usa NaN/0 si aún no hay fix) */
  double lat = gps.location.isValid() ? gps.location.lat() : NAN;
  double lon = gps.location.isValid() ? gps.location.lng() : NAN;
  uint32_t sat = gps.satellites.isValid() ? gps.satellites.value() : 0;

  uint16_t yr = gps.date.isValid() ? gps.date.year()  : 0;
  uint8_t  mo = gps.date.isValid() ? gps.date.month() : 0;
  uint8_t  dy = gps.date.isValid() ? gps.date.day()   : 0;
  uint8_t  hr = gps.time.isValid() ? gps.time.hour()  : 0;
  uint8_t  mn = gps.time.isValid() ? gps.time.minute(): 0;
  uint8_t  sc = gps.time.isValid() ? gps.time.second(): 0;

  /* Construir CSV */
  char buf[160];
  snprintf(buf, sizeof(buf),
           "%04u-%02u-%02u,%02u:%02u:%02u,%.6f,%.6f,%u,"
           "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
           yr,mo,dy, hr,mn,sc,
           lat,lon,sat,
           acc.acceleration.x, acc.acceleration.y, acc.acceleration.z,
           gyr.gyro.x,        gyr.gyro.y,        gyr.gyro.z,
           imuT.temperature,  dhtT,              dhtH);

  /* Enviar y mostrar */
  LoRa.beginPacket();
  LoRa.print(buf);
  LoRa.endPacket();

  Serial.println(buf);
}
