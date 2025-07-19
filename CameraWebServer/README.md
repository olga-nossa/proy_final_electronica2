# 📷 ESP32-S3-CAM Web Server (con OV2640)

Este proyecto implementa un servidor web para la transmisión de video desde una cámara OV2640 conectada a un ESP32-S3. Se accede al stream directamente desde cualquier navegador en la misma red Wi-Fi.


## Características

- Transmisión en vivo ESP32-S3 con módulo de cámara (como el ESP32-S3-EYE, ESP32-S3 CAM).
- Selección de resolución de imagen.
- Configuración mediante código.
- Requiere PSRAM para resoluciones altas.


## Configuración del entorno (Arduino IDE)

### Requisitos de Software:

1. **Arduino IDE** instalado (`https://www.arduino.cc/en/software`)
2. Instalar soporte para ESP32:
   - Abrir el menú: `Archivo` > `Preferencias`
   - En “Gestor de URLs adicionales de tarjetas”, agregar:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
     ```

3. Luego ir a:  
   `Herramientas` > `Gestor de placas`  
   Buscar **ESP32** e instalar el paquete de Espressif.

4. Bibliotecas necesarias:
  - `esp_camera.h`
  - `WiFi.h`


### Configuración recomendada en Arduino IDE

Desde el menú `Herramientas`:

- **Placa:** ESP32S3 Dev Module  
- **Puerto:** (COM que corresponda)  
- **USB CDC On Boot:** Enabled  
- **CPU Frequency:** 240MHz (WiFi)  
- **Flash Mode:** QIO 80MHz  
- **Flash Size:** 16MB (128Mb)  
- **PSRAM:** OPI PSRAM
- **Upload Mode:** UART0 / Hardware CDC  
- **Upload Speed:** 921600  
- **Partition Scheme:** Huge APP (3MB No OTA / 1MB SPIFFS)  
- **USB Mode:** Hardware CDC and JTAG  


## Resoluciones disponibles (FRAMESIZE)

Con la cámara OV2640, el máximo permitido es UXGA (1600x1200).
Si el video tiene latencia, se puede:

- Aumentar jpeg_quality (menor calidad).
- Reducir el frame_size (resolución más baja).

### Lista completa de resoluciones

```cpp
CONFIG FRAME SIZE     RESOLUTION   ASPECT RATIO

FRAMESIZE_96X96       96x96         1 : 1
FRAMESIZE_QQVGA       160x120       4 : 3
FRAMESIZE_QCIF        176x144      ~4 : 3
FRAMESIZE_HQVGA       240x176      ~4 : 3
FRAMESIZE_240X240     240x240       1 : 1
FRAMESIZE_QVGA        320x240       4 : 3
FRAMESIZE_CIF         352x288      ~4 : 3
FRAMESIZE_HVGA        480x320       3 : 2
FRAMESIZE_VGA         640x480       4 : 3
FRAMESIZE_SVGA        800x600       4 : 3
FRAMESIZE_XGA         1024x768      4 : 3
FRAMESIZE_HD          1280x720      16: 9
FRAMESIZE_SXGA        1280x1024     5 : 4
FRAMESIZE_UXGA        1600x1200     4 : 3 ✅ (máximo con OV2640 + PSRAM)

FRAMESIZE_FHD         1920x1080     16: 9
FRAMESIZE_P_HD        720x1280      9 :16
FRAMESIZE_P_3MP       864x1536      9 :16
FRAMESIZE_QXGA        2048x1536     4 : 3
FRAMESIZE_QHD         2560x1440     16: 9
FRAMESIZE_WQXGA       2560x1600     16:10
FRAMESIZE_P_FHD       1080x1920     9 :16
FRAMESIZE_QSXGA       2560x1920     4 : 3
```


## Configuración

1. Abrir `CameraWebServer.ino` en el Arduino IDE.
2. Configurar los valores de la red Wi-Fi:

```cpp
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";
```
