#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;

Servo servoFront;
Servo servoBack;
Servo servoLeft;
Servo servoRight;

float pitch, roll;
float setPoint = 0.0;

// PID variables
float Kp = 1.75, Ki = 0, Kd = 0;
float errorPitch, errorRoll;
float prevErrorPitch = 0, prevErrorRoll = 0;
float integralPitch = 0, integralRoll = 0;

unsigned long lastTime;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 no conectado");
    while (1);
  }

  servoFront.attach(6);
  servoBack.attach(5);
  servoLeft.attach(3);
  servoRight.attach(9);

  lastTime = millis();
}

void loop() {
  // Leer acelerómetro
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Convertir a float
  float fax = ax;
  float fay = ay;
  float faz = az;

  // Calcular ángulos (en grados)
  pitch = atan2(fax, sqrt(fay * fay + faz * faz)) * 180.0 / PI;
  roll  = atan2(fay, sqrt(fax * fax + faz * faz)) * 180.0 / PI;

  // Calcular delta tiempo
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // PID Pitch
  errorPitch = setPoint - pitch;
  integralPitch += errorPitch * dt;
  float derivativePitch = (errorPitch - prevErrorPitch) / dt;
  float outputPitch = Kp * errorPitch + Ki * integralPitch + Kd * derivativePitch;
  prevErrorPitch = errorPitch;

  // PID Roll
  errorRoll = setPoint - roll;
  integralRoll += errorRoll * dt;
  float derivativeRoll = (errorRoll - prevErrorRoll) / dt;
  float outputRoll = Kp * errorRoll + Ki * integralRoll + Kd * derivativeRoll;
  prevErrorRoll = errorRoll;

  // Mezclar salida a servos (centrado en 90°)
  int front = constrain(90 + outputPitch, 0, 180);
  int back  = constrain(90 - outputPitch, 0, 180);
  int left  = constrain(90 + outputRoll, 0, 180);
  int right = constrain(90 - outputRoll, 0, 180);

  servoFront.write(front);
  servoBack.write(back);
  servoLeft.write(left);
  servoRight.write(right);

  // Enviar al Serial Plotter
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.print(front);
  Serial.print(",");
  Serial.print(back);
  Serial.print(",");
  Serial.print(left);
  Serial.print(",");
  Serial.print(right);
  Serial.println();  // Salto de línea



  delay(10); 
}
