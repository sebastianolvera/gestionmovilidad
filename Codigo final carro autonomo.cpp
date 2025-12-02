// Pines del Sensor Ultrasónico Enfrente (e = enfrente)
const int echoe = 35; // Pin Echo: Recibe el pulso
const int trige = 32; // Pin Trig: Envía el pulso

// Pines del Sensor Ultrasónico Izquierda (i = izquierda)
const int echoi = 26;
const int trigi = 27;

// Pines del Sensor Ultrasónico Derecha (d = derecha)
const int echod = 21;
const int trigd = 23;

// Pines de Control del Motor (usualmente conectados a un L298N)
// Motor 1 (Ej: Rueda Izquierda)
const int pin2 = 2;   // Motor 1 - Pin IN1
const int pin4 = 4;   // Motor 1 - Pin IN2
const int pinEN1 = 12; // Motor 1 - Pin Enable (para control PWM/Velocidad)

// Motor 2 (Ej: Rueda Derecha)
const int pin18 = 18; // Motor 2 - Pin IN3
const int pin19 = 19; // Motor 2 - Pin IN4
const int pinEN2 = 13; // Motor 2 - Pin Enable (para control PWM/Velocidad)


// FUNCIÓN SETUP: Configuración Inicial

void setup() {
  // Inicializa la comunicación serial para depuración (imprimir el estado del robot)
  Serial.begin(115200);
  
  // Configuración de pines de Motores como SALIDA
  pinMode(pin2, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin18, OUTPUT);
  pinMode(pin19, OUTPUT);
  pinMode(pinEN1, OUTPUT); // Habilitadores de velocidad
  pinMode(pinEN2, OUTPUT); // Habilitadores de velocidad

  // Configuración de pines de Sensores Ultrasónicos
  // Pines Echo como ENTRADA (reciben el pulso de rebote)
  pinMode(echoe, INPUT);
  pinMode(echoi, INPUT);
  pinMode(echod, INPUT);
  
  // Pines Trig como SALIDA (envían el pulso de sonido)
  pinMode(trige, OUTPUT);
  pinMode(trigi, OUTPUT);
  pinMode(trigd, OUTPUT);
  
  // Asegura que todos los pines Trig estén en estado LOW (listos para el primer pulso)
  digitalWrite(trige, LOW);  
  digitalWrite(trigi, LOW);  
  digitalWrite(trigd, LOW);  
}


//FUNCIÓN LOOP: Ejecución Continua

void loop() {
  // MEDICIÓN SENSOR ENFRENTE 
  // Envía un pulso de 10us para activar el sensor
  digitalWrite(trige, LOW);  
  delayMicroseconds(2);
  digitalWrite(trige, HIGH);  
  delayMicroseconds(10); // Duración del pulso de disparo
  digitalWrite(trige, LOW);

  // Mide la duración del pulso de retorno (Echo)
  long tiempo1 = pulseIn(echoe, HIGH);
  // Calcula la distancia en centímetros: (tiempo * velocidad del sonido cm/µs) / 2
  // (0.0343 cm/µs * tiempo1) / 2
  float distancia1 = (tiempo1 * 0.0343) / 2;

  //MEDICIÓN SENSOR IZQUIERDA 
  digitalWrite(trigi, LOW);  
  delayMicroseconds(2);
  digitalWrite(trigi, HIGH);  
  delayMicroseconds(10);
  digitalWrite(trigi, LOW);

  long tiempo2 = pulseIn(echoi, HIGH);
  float distancia2 = (tiempo2 * 0.0343) / 2; // Distancia Izquierda

  //MEDICIÓN SENSOR DERECHA 
  digitalWrite(trigd, LOW);  
  delayMicroseconds(2);
  digitalWrite(trigd, HIGH);  
  delayMicroseconds(10);
  digitalWrite(trigd, LOW);

  long tiempo3 = pulseIn(echod, HIGH);
  float distancia3 = (tiempo3 * 0.0343) / 2; // Distancia Derecha

  //CONTROL DE VELOCIDAD (PWM)
  // Establece la velocidad de ambos motores a un valor de 100 (de 0 a 255)
  analogWrite(pinEN1, 100);
  analogWrite(pinEN2, 100);

  //LÓGICA DE EVASIÓN DE OBSTÁCULOS
  
  // Caso 1: Obstáculo a menos de 20cm Y hay más espacio a la DERECHA (distancia3 > distancia2)
  if (distancia1 < 20 && distancia2 < distancia3){
    // Acción 1: Retroceder (durante 1 segundo)
    // Motor 1 y Motor 2: Atrás (asumiendo esta combinación significa retroceder)
    digitalWrite(pin2,LOW);  digitalWrite(pin18,LOW);
    digitalWrite(pin4,HIGH); digitalWrite(pin19,HIGH);
    Serial.println("Atras");
    delay(1000); // Espera 1 segundo retrocediendo
    
    // Acción 2: Girar a la DERECHA (después de retroceder)
    // Motor 1 y Motor 2: Giran en la misma dirección (esto es un giro sobre su eje si están cableados opuestos)
    digitalWrite(pin2,HIGH);  digitalWrite(pin18,HIGH);
    digitalWrite(pin4,LOW); digitalWrite(pin19,LOW);
    Serial.println("Derecha");
  }
  
  // Caso 2: Obstáculo a menos de 20cm Y hay más espacio a la IZQUIERDA (distancia2 > distancia3)
  else if (distancia1 < 20 && distancia2 > distancia3){
    // Acción 1: Retroceder (durante 1 segundo)
    // Motor 1: HIGH/LOW, Motor 2: LOW/HIGH (Combinación de retroceso según tu código)
    digitalWrite(pin2,HIGH); digitalWrite(pin18,LOW);
    digitalWrite(pin4,LOW);  digitalWrite(pin19,HIGH);
    Serial.println("Atras");
    delay(1000); // Espera 1 segundo retrocediendo
    
    // Acción 2: Girar a la IZQUIERDA (después de retroceder)
    // Motor 1: LOW/HIGH, Motor 2: HIGH/LOW (Combinación de giro a la izquierda)
    digitalWrite(pin2,LOW); digitalWrite(pin18,HIGH);
    digitalWrite(pin4,HIGH);  digitalWrite(pin19,LOW);
    Serial.println("Izquierda");
  }
  
  // Caso 3: No hay obstáculo enfrente (distancia1 >= 20)
  // O los obstáculos en ambos lados son iguales o muy cercanos
  else {
    // Acción: Avanzar
    // Motor 1: HIGH/HIGH (Avanzar/Parar), Motor 2: HIGH/LOW (Avanzar)
    // **Nota:** Esta combinación puede causar que el robot avance o gire levemente
    // dependiendo del cableado específico de tu puente H.
    digitalWrite(pin2,HIGH); digitalWrite(pin18,HIGH);
    digitalWrite(pin4,HIGH); digitalWrite(pin19,LOW);
    Serial.println("Enfrente");
  }
}