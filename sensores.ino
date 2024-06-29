// Definimos los pines de los sensores infrarrojos
const int sensor1Pin = 2;
const int sensor2Pin = 3;
const int sensor3Pin = 4;
const int sensor4Pin = 5;
const int sensorLecturaPin = 6;

// Pin para enviar señal al otro Arduino
const int signalPin = 7;

struct BrailleMapping {
  const char* sequence;
  char asciiChar;
};

// Mapeo de secuencias Braille a caracteres ASCII en minúsculas
BrailleMapping brailleMap[] = {
  {"100000", 'a'},
  {"101000", 'b'},
  {"110000", 'c'},
  {"110100", 'd'},
  {"100100", 'e'},
  {"111000", 'f'},
  {"111100", 'g'},
  {"101100", 'h'},
  {"011000", 'i'},
  {"011100", 'j'},
  {"100010", 'k'},
  {"101010", 'l'},
  {"110010", 'm'},
  {"110110", 'n'},
  {"100110", 'o'},
  {"111010", 'p'},
  {"111110", 'q'},
  {"101110", 'r'},
  {"011010", 's'},
  {"011110", 't'},
  {"100011", 'u'},
  {"101011", 'v'},
  {"011101", 'w'},
  {"110011", 'x'},
  {"110111", 'y'},
  {"100111", 'z'},
  {"000000", ' '}, // Espacio
  {"000001", '.'}, // Punto
  {"000011", ','}, // Coma
  {"000101", ';'}, // Punto y coma
  {"000111", ':'}, // Dos puntos
  {"001001", '?'}, // Signo de interrogación
  {"001011", '!'}, // Signo de exclamación
  {"001101", '('}, // Paréntesis izquierdo
  {"001111", ')'}, // Paréntesis derecho
  {"010000", '1'},
  {"010100", '2'},
  {"010110", '3'},
  {"010010", '4'},
  {"011000", '5'},
  {"011100", '6'},
  {"011010", '7'},
  {"011110", '8'},
  {"100001", '9'},
  {"101001", '0'}
};

char brailleToAscii(String brailleSequence) {
  for (int i = 0; i < sizeof(brailleMap) / sizeof(brailleMap[0]); i++) {
    if (brailleSequence == brailleMap[i].sequence) {
      return brailleMap[i].asciiChar;
    }
  }
  return '?'; // Carácter por defecto si no se reconoce la secuencia
}

// Convertir un carácter ASCII a su representación en binario
char *charToBinary7Bits(char c) {
  static char bin[8];
  bin[7] = '\0';
  for (int i = 0; i < 7; i++) {
    bin[6 - i] = (c & (1 << i)) ? '1' : '0';
  }
  return bin;
}

void setup() {
  // Inicializamos la comunicación serial
  Serial.begin(9600);

  // Configuramos los pines de los sensores como entradas
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
  pinMode(sensorLecturaPin, INPUT);
  
  // Configuramos el pin de señal como salida
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW); // Inicialmente, la señal está en bajo
}

void loop() {
  // Leemos el estado de los cuatro sensores
  int sensor1State = digitalRead(sensor1Pin);
  int sensor2State = digitalRead(sensor2Pin);
  int sensor3State = digitalRead(sensor3Pin);
  int sensor4State = digitalRead(sensor4Pin);

  // Verificamos si todos los sensores detectan la tarjeta en posición
  if (sensor1State == HIGH && sensor2State == HIGH && sensor3State == HIGH && sensor4State == HIGH) {
    // Si la tarjeta está en posición, leemos el quinto sensor para la secuencia en código Braille
    String brailleSequence = "";
    for (int i = 0; i < 6; i++) {
      int sensorLecturaState = digitalRead(sensorLecturaPin);
      brailleSequence += String(sensorLecturaState);
      delay(100); // Ajusta el tiempo según sea necesario para leer los datos correctamente
    }

    // Convertir la secuencia a carácter ASCII
    char asciiChar = brailleToAscii(brailleSequence);

    // Enviamos la secuencia binaria al Arduino Mega
    Serial.println( charToBinary7Bits(asciiChar));

    // Enviamos una señal al otro Arduino para mover el motor
    digitalWrite(signalPin, HIGH);
    delay(5000); // Mantenemos la señal alta durante 100 ms
    digitalWrite(signalPin, LOW);
  } else {
    // Si la tarjeta no está en posición, mostramos un mensaje de error
    Serial.println("Tarjeta no en posición");
  }

  // Esperamos un poco antes de la siguiente lectura
  delay(5000);
}