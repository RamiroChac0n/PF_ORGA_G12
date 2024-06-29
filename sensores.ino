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

BrailleMapping brailleMap[] = {
  {"100000", 'A'},
  {"101000", 'B'},
  {"110000", 'C'},
  {"110100", 'D'},
  {"100100", 'E'},
  {"111000", 'F'},
  {"111100", 'G'},
  {"101100", 'H'},
  {"011000", 'I'},
  {"011100", 'J'},
  {"100010", 'K'},
  {"101010", 'L'},
  {"110010", 'M'},
  {"110110", 'N'},
  {"100110", 'O'},
  {"111010", 'P'},
  {"111110", 'Q'},
  {"101110", 'R'},
  {"011010", 'S'},
  {"011110", 'T'},
  {"100011", 'U'},
  {"101011", 'V'},
  {"011101", 'W'},
  {"110011", 'X'},
  {"110111", 'Y'},
  {"100111", 'Z'},
  {"010000", '1'},
  {"011000", '2'},
  {"010100", '3'},
  {"010110", '4'},
  {"010010", '5'},
  {"011100", '6'},
  {"011110", '7'},
  {"011010", '8'},
  {"001100", '9'},
  {"001110", '0'},
  {"000000", ' '},
  {"000001", '.'},
  {"000011", ','},
  {"000101", ';'},
  {"000111", ':'},
  {"000010", '!'},
  {"001000", '?'},
  {"001010", '('},
  {"001110", ')'},
  {"001001", '-'},
  {"001011", '/'},
  {"001101", '*'},
  {"001111", '+'},
  {"010001", '='},
  {"010011", '$'},
  {"010101", '%'},
  {"010111", '#'},
  {"011001", '@'},
  {"011011", '&'},
  {"011101", '_'},
  {"011111", '"'},
  {"100001", '['},
  {"100011", ']'},
  {"100101", '{'},
  {"100111", '}'},
  {"101001", '<'},
  {"101011", '>'},
  {"101101", '|'},
  {"101111", '\\'},
  {"110001", '`'},
  {"110011", '~'},
  {"110101", '^'},
  {"110111", '°'},
  {"111001", '€'},
  {"111011", '¥'},
  {"111101", '£'},
  {"111111", '¢'}
};

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

    // Convertimos la secuencia a binario
    int brailleValue = strtol(brailleSequence.c_str(), NULL, 2);
    Serial.println(brailleValue); // Enviamos la secuencia binaria al Arduino Mega

    // Enviamos una señal al otro Arduino para mover el motor
    digitalWrite(signalPin, HIGH);
    delay(5000); // Mantenemos la señal alta durante 100 ms
    digitalWrite(signalPin, LOW);
  } else {
    // Si la tarjeta no está en posición, mostramos un mensaje de error
    Serial.println("Tarjeta no en posición");
  }

  // Esperamos un poco antes de la siguiente lectura
  delay(500);
}