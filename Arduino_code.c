#include <AFMotor.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeMCU(A0, A1);

// M1 = Motore Sinistro, M2 = Motore Destro (invertili se il robot gira al contrario)
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

void setup() {
  Serial.begin(9600);      
  NodeMCU.begin(9600);    
 
  Serial.println("Arduino pronto a guidare il robot!");
 
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
}


