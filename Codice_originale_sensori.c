/*
 * Monitoraggio Qualità Acqua: pH (A0) e Ossigeno Disciolto (A1)
 */

#define PH_PIN A0
#define DO_PIN A1
#define VREF 5000    // Tensione di riferimento in mV (5V)
#define ADC_RES 1024 // Risoluzione ADC Arduino Uno

// --- Variabili pH ---
float phOffset = 0.00; // Regola questo valore dopo la calibrazione (es. -0.15)

// --- Variabili Ossigeno Disciolto (DO) ---
// Valore tipico: a 25°C l'aria satura (100%) dà circa 1600mV
// e corrisponde a ~8.26 mg/L di ossigeno.
float doCalibrationVoltage = 1600.0;
float doSaturationMgL = 8.26;

void setup() {
  Serial.begin(9600);
  Serial.println("--- Sistema Avviato: pH e DO ---");
}

void loop() {
  // 1. Lettura pH (Media di 10 campioni per stabilità)
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

  delay(2000); // Attesa tra le letture
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