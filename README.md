# Servo Controller ESP32

## Descripción

Sistema de control de servo motor basado en ESP32 que permite el control dual mediante potenciómetro físico y comandos Bluetooth. El sistema implementa detección inteligente de modo de control, filtrado de señal y comunicación bidireccional.

## Características

### Control dual
- **Modo manual**: control directo mediante potenciómetro
- **Modo remoto**: control vía Bluetooth con comandos específicos
- **Conmutación automática**: detección inteligente del modo activo

### Conectividad Bluetooth
- Nombre del dispositivo: `ServoController_ESP32`
- Protocolo de comandos personalizado
- Retroalimentación de posición en tiempo real
- Timeout automático para comandos remotos

### Procesamiento de señal
- Filtro de media móvil para el potenciómetro (5 muestras)
- Detección de movimiento con umbral configurable
- Suavizado de movimientos del servo

## Especificaciones técnicas

### Hardware requerido
- **Microcontrolador**: ESP32 38 pinout
- **Servo Motor**: Compatible con señal PWM estándar (0-180°)
- **Potenciómetro**: 10kΩ (recomendado)
- **Alimentación**: 5V para servo, 3.3V para ESP32 (Cable de alimentación USB)

### Pines de Conexión
```
Pin 34 (ADC1_CH6) → Potenciómetro (entrada analógica)
Pin 4 (GPIO4)     → Servo Motor (señal PWM)
```

### Rango de operación
- **Ángulo de Servo**: 0° - 180°
- **Resolución ADC**: 12 bits (0-4095)
- **Frecuencia de actualización**: 50Hz (20ms)

## Instalación

### Librerías Requeridas
```cpp
#include <ESP32Servo.h>
#include "BluetoothSerial.h"
```

### Configuración del entorno
1. Instalar el core ESP32 en Arduino IDE
2. Instalar la librería `ESP32Servo`
3. Habilitar Bluetooth en la configuración del ESP32

### Compilación
```bash
# Configuración recomendada
Board: ESP32 Dev Module
CPU Frequency: 240MHz
Flash Size: 4MB
Partition Scheme: Default 4MB
```

## Protocolo de comunicación

### Comandos bluetooth (Entrada)
```
ANGLE:<valor>
```
- `<valor>`: Ángulo deseado (0-180)
- Ejemplo: `ANGLE:90`

### Respuestas bluetooth (Salida)
```
POSITION:<ángulo_actual>
```
- `<ángulo_actual>`: Posición actual del servo
- Ejemplo: `POSITION:90`

## Configuración avanzada

### Parámetros ajustables
```cpp
const unsigned long TIMEOUT_BT = 2000;     // Timeout Bluetooth (ms)
const int NUM_LECTURAS = 5;                // Muestras para filtro
const int umbralMovimiento = 5;            // Sensibilidad potenciómetro
const int umbralServo = 2;                 // Precisión movimiento servo
```

### Calibración del potenciómetro
El sistema mapea automáticamente el rango completo del ADC (0-4095) al rango del servo (0-180°). Para calibración personalizada, modificar:

```cpp
anguloPotenciometro = map(promedio, valorMin, valorMax, 0, 180);
```

## Funcionamiento

### Algoritmo de control
1. **Lectura continua**: muestreo del potenciómetro cada 20ms
2. **Filtrado**: aplicación de media móvil para reducir ruido
3. **Detección de Modo**: conmutación automática entre control manual y remoto
4. **Actualización**: movimiento del servo solo cuando hay cambios significativos

### Estados del sistema
- **Modo POT**: control activo por potenciómetro
- **Modo BT**: control activo por Bluetooth
- **Transición**: cambio automático según última entrada detectada

## Monitoreo y depuración

### Salida serie (115200 baud)
```
Modo: BT | Angulo: 90 | Pot: 2048 (90°)
```

### Información mostrada
- **Modo**: modo de control actual (BT/POT)
- **Angulo**: posición actual del servo
- **Pot**: valor raw y convertido del potenciómetro

## Ejemplos de uso

### Aplicación Móvil
Conectar via Bluetooth y enviar comandos:
```
ANGLE:0    // Mover a 0°
ANGLE:90   // Mover a 90°
ANGLE:180  // Mover a 180°
```

### Control terminal
```bash
# Conexión via terminal serie
echo "ANGLE:45" > /dev/rfcomm0
```

## Solución de problemas

### Problemas comunes
1. **Servo no responde**: verificar conexión pin 4 y alimentación
2. **Potenciómetro ruidoso**: ajustar NUM_LECTURAS o umbral
3. **Bluetooth no conecta**: verificar emparejamiento y nombre del dispositivo

### Diagnóstico
- Monitorear salida serie para valores de depuración
- Verificar rangos de valores del potenciómetro
- Comprobar estabilidad de la alimentación

## Licencia

Este proyecto está bajo licencia MIT. Consulte el archivo LICENSE para más detalles.

## Autor

- [@05545](https://www.github.com/05545)
