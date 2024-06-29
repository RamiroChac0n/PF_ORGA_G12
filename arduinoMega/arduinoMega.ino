#include <Wire.h>

// Pines de entrada para los datos de Braille
const int dataPins[7] = {22, 23, 24, 25, 26, 27, 28};
const int clockPin = 29;  // Pin para enviar la señal de reloj y habilitar la escritura

// Pines de salida para las variables A, B, y C
const int APin = 30;
const int BPin = 31;
const int CPin = 32;

// Pin de entrada para el comando de reset
const int resetInputPin = 33; // Cambia este número al pin que quieras usar

// Configuración de la RAM de 7x8
const int rows = 7;
const int cols = 8;
int brailleData[rows][cols];

// Variables para asignar la posición de memoria
int A = 0;
int B = 0;
int C = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Inicializamos la comunicación I2C si es necesario

  // Configurar pines de datos como entradas
  for (int i = 0; i < 7; i++) {
    pinMode(dataPins[i], INPUT);
  }
  pinMode(clockPin, OUTPUT);

  // Configurar pines de salida para A, B, y C
  pinMode(APin, OUTPUT);
  pinMode(BPin, OUTPUT);
  pinMode(CPin, OUTPUT);

  // Configurar pin de entrada para el comando de reset
  pinMode(resetInputPin, INPUT);

  // Inicializamos la RAM de 7x8 a 0
  clearMemory();
}

void loop() {
  // Leer datos de forma serial desde el Arduino
  if (Serial.available() >= 7) {
    String inputData = Serial.readStringUntil('\n');
    if (inputData.length() == 7) {
      int value = binaryStringToInt(inputData);
      writeToMemory(value);
    }
  }

  // Verificar si la memoria está llena y enviar los datos a Python
  if (isMemoryFull()) {
    sendMemoryToPython();
    clearMemory();
  }

  // Verificar si se ha recibido un comando de reset desde el pin de entrada
  if (digitalRead(resetInputPin) == HIGH) {
    clearMemory();
    delay(500); // Añadir un pequeño delay para evitar múltiples resets consecutivos
  }

  delay(1000); // Esperamos un segundo antes de la siguiente lectura
}

int binaryStringToInt(String binaryString) {
  int value = 0;
  for (int i = 0; i < 7; i++) {
    value |= (binaryString.charAt(i) == '1' ? 1 : 0) << i;
  }
  return value;
}

void writeToMemory(int value) {
  brailleData[A][B] = value;
  incrementMemoryPosition();
  digitalWrite(clockPin, HIGH);  // Enviar pulso de reloj/escritura
  delay(1);
  digitalWrite(clockPin, LOW);
}

void incrementMemoryPosition() {
  C++;
  if (C >= cols) {
    C = 0;
    B++;
    if (B >= cols) {
      B = 0;
      A++;
      if (A >= rows) {
        A = 0;
      }
    }
  }
  updateAddressPins();
}

bool isMemoryFull() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (brailleData[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

void sendMemoryToPython() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Serial.print(brailleData[i][j]);
      if (j < cols - 1) Serial.print(",");
    }
    Serial.println();
  }
}

void clearMemory() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      brailleData[i][j] = 0;
    }
  }
  A = 0;
  B = 0;
  C = 0;
  updateAddressPins();
}

void updateAddressPins() {
  digitalWrite(APin, A);
  digitalWrite(BPin, B);
  digitalWrite(CPin, C);
}
