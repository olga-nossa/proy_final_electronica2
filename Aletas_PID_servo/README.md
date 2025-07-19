# ⚖️ Plataforma de Estabilización 2D con MPU6050 y PID

Este proyecto implementa un sistema de control de posición en dos ejes (Pitch y Roll) utilizando un MPU6050 y cuatro servomotores. El sistema se estabiliza mediante controladores PID independientes para cada eje.

---

## Objetivo

Estabilizar una plataforma en posición horizontal utilizando entradas del sensor de aceleración y actuadores (servos) que contrarrestan el movimiento.

---

## Componentes necesarios

- Placa Arduino UNO / Nano / Mega
- Sensor **MPU6050**
- 4x **Servomotores**
- Fuente externa para los servos (recomendado)
- Cables jumper

---

## Librerías requeridas

Instalar desde el Gestor de Librerías:

- [`MPU6050`](https://github.com/jrowberg/i2cdevlib) (de Jeff Rowberg)
- [`Servo`](https://www.arduino.cc/en/Reference/Servo)
- `Wire` (incluida por defecto)

---

## Conexiones

| Elemento       | Arduino UNO |
|----------------|-------------|
| MPU6050 SDA    | A4          |
| MPU6050 SCL    | A5          |
| Servo Front    | D6          |
| Servo Back     | D5          |
| Servo Left     | D3          |
| Servo Right    | D9          |

---

## Lógica de control

- Se mide el **ángulo de Pitch y Roll** usando la aceleración del MPU6050.
- Se aplica **PID** por separado para cada eje.
- Los servos responden con una corrección proporcional al error.
- La posición base de cada servo es `90°`, ajustándose según la salida del PID.

```cpp
output = Kp * error + Ki * integral + Kd * derivative;
servo.write(90 ± output);
