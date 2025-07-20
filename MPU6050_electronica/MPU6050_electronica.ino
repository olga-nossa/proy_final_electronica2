#include <Wire.h>

const int MPU_ADDR = 0x68;

int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
float accAngleX, accAngleY, gyroAngleX = 0, gyroAngleY = 0, gyroAngleZ = 0;
float angleX = 0, angleY = 0, angleZ = 0;
float elapsedTime, currentTime, previousTime;

const float GYRO_SENSITIVITY = 131.0;
const float ALPHA = 0.96;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  previousTime = millis();
  delay(100);
}

void loop() {
  // Leer datos
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  // Mostrar valores crudos (opcional para debug)
  /*
  Serial.print("accX: "); Serial.print(accX);
  Serial.print(" accY: "); Serial.print(accY);
  Serial.print(" accZ: "); Serial.println(accZ);
  */

  // Evitar división por cero
  float denX = sqrt(pow(accX, 2) + pow(accZ, 2));
  float denY = sqrt(pow(accY, 2) + pow(accZ, 2));
  if (denX < 1e-6) denX = 1e-6;
  if (denY < 1e-6) denY = 1e-6;

  accAngleX = atan(accY / denX) * 180 / PI;
  accAngleY = atan(-accX / denY) * 180 / PI;

  // Tiempo
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000.0;
  if (elapsedTime <= 0) elapsedTime = 0.01;
  previousTime = currentTime;

  // Giroscopio
  gyroAngleX += (gyroX / GYRO_SENSITIVITY) * elapsedTime;
  gyroAngleY += (gyroY / GYRO_SENSITIVITY) * elapsedTime;
  gyroAngleZ += (gyroZ / GYRO_SENSITIVITY) * elapsedTime;

  // Filtro complementario
  angleX = ALPHA * (angleX + (gyroX / GYRO_SENSITIVITY) * elapsedTime) + (1 - ALPHA) * accAngleX;
  angleY = ALPHA * (angleY + (gyroY / GYRO_SENSITIVITY) * elapsedTime) + (1 - ALPHA) * accAngleY;
  angleZ += (gyroZ / GYRO_SENSITIVITY) * elapsedTime;

  // Imprimir
  Serial.print("Angle X: "); Serial.print(angleX);
  Serial.print(" | Angle Y: "); Serial.print(angleY);
  Serial.print(" | Angle Z: "); Serial.println(angleZ);

  delay(100);
}

