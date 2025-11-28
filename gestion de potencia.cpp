// Definición de Pines de Sensores Ultrasónicos
// Sensores: Enfrente (e), Izquierda (i), Derecha (d)
const int echoe = 35;
const int trige = 32;
const int echoi = 26;
const int trigi = 27;
const int echod = 21;
const int trigd = 23;

// Pines del Controlador de Motor (Puente H)
// Motor Izquierdo: Control de Dirección (pin2, pin4) y Habilitación (pinEN1)
const int pinM1A = 2; // IN1
const int pinM1B = 4; // IN2
const int pinEN1 = 12; // Enable 1 (Velocidad/PWM)

// Motor Derecho: Control de Dirección (pin18, pin19) y Habilitación (pinEN2)
const int pinM2A = 18; // IN3
const int pinM2B = 19; // IN4
const int pinEN2 = 13; // Enable 2 (Velocidad/PWM)

// Variables globales para las distancias
float distanciaFrontal = 0.0;
float distanciaIzquierda = 0.0;
float distanciaDerecha = 0.0;

// --- Funciones de Movimiento ---

// **Gestión de Potencia (Velocidad)**
void establecerVelocidad(int velocidad) {
  // Establece la velocidad de ambos motores (0-255)
  analogWrite(pinEN1, velocidad);
  analogWrite(pinEN2, velocidad);
}

// **Gestión de Movilidad (Dirección)**
void moverAdelante() {
  digitalWrite(pinM1A, HIGH);
  digitalWrite(pinM1B, LOW);
  digitalWrite(pinM2A, HIGH);
  digitalWrite(pinM2B, LOW);
  Serial.println("Movilidad: Enfrente");
}

void moverAtras() {
  digitalWrite(pinM1A, LOW);
  digitalWrite(pinM1B, HIGH);
  digitalWrite(pinM2A, LOW);
  digitalWrite(pinM2B, HIGH);
  Serial.println("Movilidad: Atras");
}

void girarDerecha() {
  // Gira sobre su eje, M1 adelante, M2 atras
  digitalWrite(pinM1A, HIGH);
  digitalWrite(pinM1B, LOW);
  digitalWrite(pinM2A, LOW);
  digitalWrite(pinM2B, HIGH);
  Serial.println("Movilidad: Girar Derecha");
}

void girarIzquierda() {
  // Gira sobre su eje, M1 atras, M2 adelante
  digitalWrite(pinM1A, LOW);
  digitalWrite(pinM1B, HIGH);
  digitalWrite(pinM2A, HIGH);
  digitalWrite(pinM2B, LOW);
  Serial.println("Movilidad: Girar Izquierda");
}

void detener() {
  digitalWrite(pinM1A, LOW);
  digitalWrite(pinM1B, LOW);
  digitalWrite(pinM2A, LOW);
  digitalWrite(pinM2B, LOW);
  Serial.println("Movilidad: Detenido");
}

// --- Funciones de Sensores ---

float medirDistancia(int trigPin, int echoPin) {
  // Generar pulso de activación
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo del pulso de rebote
  long tiempo = pulseIn(echoPin, HIGH);

  // Calcular distancia en cm: Distancia = (Tiempo * Velocidad del Sonido) / 2
  // Velocidad del sonido = 0.0343 cm/µs
  float distancia = (tiempo * 0.0343) / 2;
  return distancia;
}

void leerTodosLosSensores() {
  distanciaFrontal = medirDistancia(trige, echoe);
  distanciaIzquierda = medirDistancia(trigi, echoi);
  distanciaDerecha = medirDistancia(trigd, echod);
  
  // Imprimir distancias (opcional, para depuración)
  Serial.print("F:"); Serial.print(distanciaFrontal);
  Serial.print("cm, I:"); Serial.print(distanciaIzquierda);
  Serial.print("cm, D:"); Serial.print(distanciaDerecha);
  Serial.println("cm");
}

// --- Setup y Loop ---

void setup() {
  Serial.begin(115200);

  // Pines de control de motor (Gestión de Movilidad)
  pinMode(pinM1A, OUTPUT);
  pinMode(pinM1B, OUTPUT);
  pinMode(pinM2A, OUTPUT);
  pinMode(pinM2B, OUTPUT);
  
  // Pines de habilitación/velocidad (Gestión de Potencia)
  pinMode(pinEN1, OUTPUT);
  pinMode(pinEN2, OUTPUT);

  // Pines de Sensores
  pinMode(echoe, INPUT);
  pinMode(trige, OUTPUT);
  pinMode(echoi, INPUT);
  pinMode(trigi, OUTPUT);
  pinMode(echod, INPUT);
  pinMode(trigd, OUTPUT);

  // Inicializar pines Trig en LOW
  digitalWrite(trige, LOW);
  digitalWrite(trigi, LOW);
  digitalWrite(trigd, LOW);
  
  detener(); // Asegurar que los motores estén detenidos al inicio
}

void loop() {
  // 1. Lectura de Sensores
  leerTodosLosSensores();
  
  // 2. Gestión de Potencia
  // La velocidad se establece en un valor constante aquí (por ejemplo, 100/255)
  // Se podría ajustar dinámicamente según la situación si fuera necesario.
  const int VELOCIDAD_BASE = 100;
  establecerVelocidad(VELOCIDAD_BASE);

  // 3. Gestión de Movilidad (Lógica de Evasión de Obstáculos)
  const float DISTANCIA_UMBRAL = 20.0; // Umbral de proximidad en cm

  if (distanciaFrontal < DISTANCIA_UMBRAL) {
    // Hay un obstáculo cerca
    detener(); // Parar antes de tomar una decisión
    delay(50); // Pequeña pausa para asegurar la parada

    if (distanciaIzquierda > DISTANCIA_UMBRAL && distanciaIzquierda > distanciaDerecha) {
      // El camino de la izquierda está libre y es más amplio que la derecha
      moverAtras();
      delay(500); // Retrocede un poco
      girarIzquierda();
      delay(700); // Gira para evitar el obstáculo
      
    } else if (distanciaDerecha > DISTANCIA_UMBRAL && distanciaDerecha > distanciaIzquierda) {
      // El camino de la derecha está libre y es más amplio que la izquierda
      moverAtras();
      delay(500); // Retrocede un poco
      girarDerecha();
      delay(700); // Gira para evitar el obstáculo
      
    } else {
      // Ambos lados bloqueados o inciertos, intentar retroceder y girar al azar (ej. Izquierda)
      moverAtras();
      delay(1000); 
      girarIzquierda();
      delay(1000); 
    }
  } else {
    // Si no hay obstáculo enfrente, avanzar
    moverAdelante();
  }
  
  delay(100); // Pequeño retardo para evitar lecturas demasiado rápidas
}