
// PINES Y VARIABLES
DEFINIR echoe COMO 35    // Sensor Enfrente (Echo)
DEFINIR trige COMO 32    // Sensor Enfrente (Trig)
DEFINIR echoi COMO 26    // Sensor Izquierda (Echo)
DEFINIR trigi COMO 27    // Sensor Izquierda (Trig)
DEFINIR echod COMO 21    // Sensor Derecha (Echo)
DEFINIR trigd COMO 23    // Sensor Derecha (Trig)

DEFINIR pin2 COMO 2      // Motor 1 - Pin A
DEFINIR pin4 COMO 4      // Motor 1 - Pin B
DEFINIR pin18 COMO 18    // Motor 2 - Pin A
DEFINIR pin19 COMO 19    // Motor 2 - Pin B
DEFINIR pinEN1 COMO 12   // Motor 1 - Habilitar/Velocidad (PWM)
DEFINIR pinEN2 COMO 13   // Motor 2 - Habilitar/Velocidad (PWM)


// MonitorSERIAL

INICIO SETUP: // Comunicación Serial
INICIAR COMUNICACION SERIAL A 115200 BAUDIOS
// Configuración de Pines de Motores (SALIDA)
CONFIGURAR pin2 COMO SALIDA; CONFIGURAR pin4 COMO SALIDA
CONFIGURAR pin18 COMO SALIDA; CONFIGURAR pin19 COMO SALIDA
CONFIGURAR pinEN1 COMO SALIDA; CONFIGURAR pinEN2 COMO SALIDA

// Configuración de Pines de Sensores (TRIG=SALIDA, ECHO=ENTRADA)
CONFIGURAR echoe COMO ENTRADA; CONFIGURAR trige COMO SALIDA
CONFIGURAR echoi COMO ENTRADA; CONFIGURAR trigi COMO SALIDA
CONFIGURAR echod COMO ENTRADA; CONFIGURAR trigd COMO SALIDA

// Inicializar Pines Trig en BAJO
ESTABLECER trige A BAJO; ESTABLECER trigi A BAJO; ESTABLECER trigd A BAJO
FIN SETUP

//  CICLO LOOP

INICIO LOOP:
// Medir la distancia
// Función auxiliar para obtener la distancia de un sensor:
FUNCIÓN CALCULAR_DISTANCIA(PinTrig, PinEcho):
ESTABLECER PinTrig A BAJO; ESPERAR 2 MICROSEGUNDOS
ESTABLECER PinTrig A ALTO; ESPERAR 10 MICROSEGUNDOS
ESTABLECER PinTrig A BAJO
    
//LEER EN PinEcho EL TIEMPO tiempo (duración del pulso ALTO)
DEVOLVER (tiempo * 0.0343) / 2
FIN FUNCIÓN
CALCULAR distancia1 = CALCULAR_DISTANCIA(trige, echoe) // Enfrente
CALCULAR distancia2 = CALCULAR_DISTANCIA(trigi, echoi) // Izquierda
CALCULAR distancia3 = CALCULAR_DISTANCIA(trigd, echod) // Derecha

//Controlar velocidad con PWM
ESTABLECER pinEN1 A 100 // Velocidad Motor 1
ESTABLECER pinEN2 A 100 // Velocidad Motor 2

  // Logica del movimiento

  SI (distancia1 < 20) ENTONCES
    // Obstáculo detectado de cerca (menos de 20 cm)

SI (distancia2 < distancia3) ENTONCES
 // Giro a la Derecha (Más espacio a la derecha)
      
// Retroceder
ESTABLECER pin2 A BAJO; ESTABLECER pin18 A BAJO
ESTABLECER pin4 A ALTO; ESTABLECER pin19 A ALTO
IMPRIMIR EN SERIAL "Atras"
ESPERAR 1000 MILISEGUNDOS

// Girar a la Derecha
ESTABLECER pin2 A ALTO; ESTABLECER pin18 A ALTO
ESTABLECER pin4 A BAJO; ESTABLECER pin19 A BAJO
IMPRIMIR EN SERIAL "Derecha"

SINO SI (distancia2 > distancia3) ENTONCES
//Giro a la Izquierda (Más espacio a la izquierda)
      
// Retroceder
ESTABLECER pin2 A ALTO; ESTABLECER pin18 A BAJO // (Movimiento de retroceso según tu código)
ESTABLECER pin4 A BAJO; ESTABLECER pin19 A ALTO
IMPRIMIR EN SERIAL "Atras"
ESPERAR 1000 MILISEGUNDOS

// Girar a la Izquierda
ESTABLECER pin2 A BAJO; ESTABLECER pin18 A ALTO
ESTABLECER pin4 A ALTO; ESTABLECER pin19 A BAJO
IMPRIMIR EN SERIAL "Izquierda"

FIN SI // Fin: Evaluación Izquierda vs Derecha

  SINO
//Campo libre: Avanzar
ESTABLECER pin2 A ALTO; ESTABLECER pin18 A ALTO
ESTABLECER pin4 A ALTO; ESTABLECER pin19 A BAJO // (Movimiento de avance según tu código)
IMPRIMIR EN SERIAL "Enfrente"

// Fin: Evaluación distancia1
FIN LOOP