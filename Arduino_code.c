#include <AFMotor.h>
#include <SoftwareSerial.h>

// --- Configurazione Sensori ---
// Cambio i pin dei sensori in A2 e A3 per evitare conflitti con la SoftwareSerial (A0, A1)
#define PH_PIN A2
#define DO_PIN A3
#define VREF 5000    // Tensione di riferimento in mV (5V)
#define ADC_RES 1024 // Risoluzione ADC Arduino Uno

float phOffset = 0.00; // Regola questo valore dopo la calibrazione
float doCalibrationVoltage = 1600.0;
float doSaturationMgL = 8.26;

unsigned long lastSensorRead = 0;

SoftwareSerial NodeMCU(A0, A1);

// M1 = Motore Sinistro, M2 = Motore Destro (invertili se il robot gira al contrario)
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

void setup() {
  Serial.begin(9600);      
  NodeMCU.begin(9600);    
 
  Serial.println("Arduino pronto a guidare il robot!");
  Serial.println("--- Sistema Avviato: pH e DO ---");
 
  motor1.setSpeed(200);    
  motor2.setSpeed(200);

  motor1.run(RELEASE);      
  motor2.run(RELEASE);
}

void loop() {
  if (NodeMCU.available()) {
    char comando = NodeMCU.read();
    Serial.print("Comando ricevuto: ");
    Serial.println(comando);

    if (comando == 'F') { // AVANTI
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    }
    else if (comando == 'B') { // INDIETRO
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
    }
    else if (comando == 'L') { // SINISTRA (M1 fermo, M2 avanti)
      motor1.run(RELEASE);
      motor2.run(FORWARD);
    }
    else if (comando == 'R') { // DESTRA (M1 avanti, M2 fermo)
      motor1.run(FORWARD);
      motor2.run(RELEASE);
    }
    else if (comando == 'S') { // STOP
      motor1.run(RELEASE);
      motor2.run(RELEASE);
    }
  }

  // Lettura sensori ogni 2 secondi, usando millis() per non bloccare la ricezione dei comandi
  if (millis() - lastSensorRead >= 2000) {
    lastSensorRead = millis();

    // 1. Lettura pH
    float phVoltage = analogReadAverage(PH_PIN) * (float)VREF / ADC_RES;
    float phValue = 3.5 * (phVoltage / 1000.0) + phOffset;

    // 2. Lettura Ossigeno Disciolto (DO)
    float doVoltage = analogReadAverage(DO_PIN) * (float)VREF / ADC_RES;
    float doValue = (doVoltage / doCalibrationVoltage) * doSaturationMgL;

    // 3. Stampa dei risultati
    Serial.print("pH: ");
    Serial.print(phValue, 2);
    Serial.print(" | ");
  
    Serial.print("DO: ");
    Serial.print(doValue, 2);
    Serial.println(" mg/L");

    // 4. Invia i dati al NodeMCU separati da un punto e virgola
    NodeMCU.print(phValue, 2);
    NodeMCU.print(";");
    NodeMCU.println(doValue, 2);
  }
}

// Funzione per leggere una media e filtrare il rumore elettronico
float analogReadAverage(int pin) {
  unsigned long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(10);
  }
  return (float)sum / 10.0;
}


