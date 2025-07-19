#include <Adafruit_MPU6050.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) { Serial.println("MPU6050 not found"); while (true) delay(10); }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);   // ±8 g
  mpu.setGyroRange        (MPU6050_RANGE_500_DEG); // ±500 °/s
  mpu.setFilterBandwidth  (MPU6050_BAND_21_HZ);    // 21 Hz
  delay(100);
}

void loop() {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  Serial.printf(
    "Acc (m/s²):  %.2f  %.2f  %.2f\n"
    "Gyro(rad/s): %.2f  %.2f  %.2f\n"
    "Temp(°C):    %.2f\n\n",
    a.acceleration.x, a.acceleration.y, a.acceleration.z,
    g.gyro.x,        g.gyro.y,        g.gyro.z,
    t.temperature
  );
  delay(1000);
}
