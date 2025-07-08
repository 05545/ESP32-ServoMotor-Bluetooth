#include <ESP32Servo.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
const int pinPotenciometro = 34;
const int pinServo = 4;
Servo servo;
int anguloServo = 90;
int anguloAnterior = -1;

int entradaPotenciometro = 0;
int anguloPotenciometro = 90;
int lecturaAnteriorPot = 0;
unsigned long ultimoMovimientoPot = 0;

String comandoBT = "";
bool controlPorBT = false;
int anguloObjetivo = 90;
unsigned long ultimoComandoBT = 0;
const unsigned long TIMEOUT_BT = 2000;

const int NUM_LECTURAS = 5;
int lecturas[NUM_LECTURAS];
int indiceLectura = 0;
int total = 0;
int promedio = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ServoController_ESP32");
  Serial.println("Dispositivo listo para emparejamiento");
  
  pinMode(pinPotenciometro, INPUT);
  servo.attach(pinServo);
  servo.write(anguloServo);
  
  for (int i = 0; i < NUM_LECTURAS; i++) {
    lecturas[i] = analogRead(pinPotenciometro);
    total += lecturas[i];
  }
  
  promedio = total / NUM_LECTURAS;
  anguloPotenciometro = map(promedio, 0, 4095, 0, 180);
  lecturaAnteriorPot = anguloPotenciometro;
}

void loop() {
  manejarBluetooth();
  leerPotenciometro();
  determinarModoControl();
  moverServo();
  delay(20);
}

void manejarBluetooth() {
  if (SerialBT.available()) {
    comandoBT = SerialBT.readStringUntil('\n');
    comandoBT.trim();
    
    if (comandoBT.startsWith("ANGLE:")) {
      int nuevoAngulo = comandoBT.substring(6).toInt();
      if (nuevoAngulo >= 0 && nuevoAngulo <= 180) {
        anguloObjetivo = nuevoAngulo;
        controlPorBT = true;
        ultimoComandoBT = millis();
        Serial.print("Comando BT recibido: ");
        Serial.println(anguloObjetivo);
      }
    }
  }
}

void leerPotenciometro() {
  total = total - lecturas[indiceLectura];
  lecturas[indiceLectura] = analogRead(pinPotenciometro);
  
  total = total + lecturas[indiceLectura];
  promedio = total / NUM_LECTURAS;

  anguloPotenciometro = map(promedio, 0, 4095, 0, 180);
  indiceLectura = (indiceLectura + 1) % NUM_LECTURAS;
}

void determinarModoControl() {
  if (abs(anguloPotenciometro - lecturaAnteriorPot) > 5) {
    ultimoMovimientoPot = millis();
    lecturaAnteriorPot = anguloPotenciometro;
    controlPorBT = false;
  }
  
  if (controlPorBT) {
    anguloServo = anguloObjetivo;
  } else {
    anguloServo = anguloPotenciometro;
  }
}

void moverServo() {
  if (abs(anguloServo - anguloAnterior) > 2) {
    servo.write(anguloServo);
    anguloAnterior = anguloServo;

    SerialBT.print("POSITION:");
    SerialBT.println(anguloServo);
    
    Serial.print("Modo: ");
    Serial.print(controlPorBT ? "BT" : "POT");
    Serial.print(" | Angulo: ");
    Serial.print(anguloServo);
    Serial.print(" | Pot: ");
    Serial.print(promedio);
    Serial.print(" (");
    Serial.print(anguloPotenciometro);
    Serial.println("°)");
  }
}