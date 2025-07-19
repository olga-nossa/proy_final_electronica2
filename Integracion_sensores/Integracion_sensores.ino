/*  ESP32 DevKit v1
    IMU (MPU‑6050) + DHT11 + LoRa SX1278 433 MHz
    -------------------------------------------------
    I²C   : SDA 21, SCL 22
    LoRa  : SS 5,  RST 34*, DIO0 35        (*RST no se usa como salida)
    DHT11 : GPIO 18
    Salida CSV: ax,ay,az,gx,gy,gz,imuTemp,dhtTemp,dhtHum
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <DHT.h>
#include <SPI.h>
#include <LoRa.h>

/* ─── Pines ─── */
constexpr uint8_t LORA_SS   = 5;
constexpr uint8_t LORA_RST  = 34;   // GPIO 34 es solo‑entrada; el módulo se auto‑reseteará
constexpr uint8_t LORA_DIO0 = 35;
constexpr long    LORA_FREQ = 433E6;

constexpr uint8_t DHTPIN  = 4;
constexpr uint8_t DHTTYPE = DHT11;

/* ─── Objetos ─── */
Adafruit_MPU6050 mpu;
DHT dht(DHTPIN, DHTTYPE);

/* ─── Setup ─── */
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22, 400000);          // I²C rápido

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found"); while (true);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange        (MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth  (MPU6050_BAND_21_HZ);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed"); while (true);
  }

  dht.begin();
  Serial.println("System ready");
}

/* ─── Loop ─── */
void loop() {
  sensors_event_t acc, gyr, imuT;
  mpu.getEvent(&acc, &gyr, &imuT);

  float dhtH = dht.readHumidity();
  float dhtT = dht.readTemperature();
  if (isnan(dhtH) || isnan(dhtT)) { dhtH = dhtT = NAN; }

  /* Construir cadena CSV */
  char buf[128];
  snprintf(buf, sizeof(buf),
           "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
           acc.acceleration.x, acc.acceleration.y, acc.acceleration.z,
           gyr.gyro.x,        gyr.gyro.y,        gyr.gyro.z,
           imuT.temperature,  dhtT,              dhtH);

  /* Enviar y mostrar */
  LoRa.beginPacket();
  LoRa.print(buf);
  LoRa.endPacket();

  Serial.println(buf);
  delay(1000);                         // 1 Hz
}