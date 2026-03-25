#include <ESP8266WiFi.h>

const char* ssid = "Barchetta_Wifi"; // Nome della rete Wi-Fi che creerà il NodeMCU
const char* password = "password123"; // Password per collegarsi (minimo 8 caratteri)

WiFiServer server(80);

// Variabili per memorizzare gli ultimi dati ricevuti dall'Arduino
String current_pH = "--";
String current_DO = "--";

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.println();
  Serial.print("Creazione Hotspot Wi-Fi: ");
  Serial.println(ssid);

  // Imposta il NodeMCU in modalità Access Point (Hotspot)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("");
  Serial.println("Hotspot Creato con successo!");
  Serial.print("Indirizzo IP per il telecomando: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  // --- Lettura dati sensori dall'Arduino tramite Serial ---
  // L'Arduino invia i dati nel formato: "pH;DO\n"
  if (Serial.available()) {
    String sensorData = Serial.readStringUntil('\n');
    sensorData.trim();
    
    int separatorIndex = sensorData.indexOf(';');
    if (separatorIndex != -1) {
      current_pH = sensorData.substring(0, separatorIndex);
      current_DO = sensorData.substring(separatorIndex + 1);
    }
  }

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  // Decodifica i nuovi comandi
  if (request.indexOf("/AVANTI") != -1)   { Serial.print('F'); }
  if (request.indexOf("/INDIETRO") != -1) { Serial.print('B'); }
  if (request.indexOf("/SINISTRA") != -1) { Serial.print('L'); }
  if (request.indexOf("/DESTRA") != -1)   { Serial.print('R'); }
  if (request.indexOf("/STOP") != -1)     { Serial.print('S'); }

  // Nuova pagina web con layout a croce e visualizzazione dati sensori
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  // Refresca la pagina in automatico ogni 5 secondi, o usa javascript per aggiornarla. 
  // Per ora un meta refresh è la soluzione più semplice e robusta per Arduino.
  client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<style>");
  client.println("  .sensor-box { border: 2px solid #333; padding: 10px; margin: 20px auto; width: 80%; max-width: 300px; border-radius: 10px; background-color: #f9f9f9; }");
  client.println("  .val { font-size: 24px; font-weight: bold; color: #0066cc; }");
  client.println("</style>");
  client.println("</head>"); 
  client.println("<body style='text-align:center; font-family:Arial;'>");
  client.println("<h1>Telecomando Rover</h1>");
  
  // Box dati sensori
  client.println("<div class='sensor-box'>");
  client.println("<h3>Dati Acqua</h3>");
  client.println("<p>pH: <span class='val'>" + current_pH + "</span></p>");
  client.println("<p>Ossigeno (DO): <span class='val'>" + current_DO + " mg/L</span></p>");
  // Link per ricaricare la pagina manualmente se serve
  client.println("<a href='/'><button style='padding:5px 15px;'>Aggiorna Dati</button></a>");
  client.println("</div>");

  client.println("<br>");
 
  // Creiamo una tabella per allineare i bottoni come un joypad
  client.println("<table style='margin:auto;'>");
  // Riga 1: Avanti
  client.println("<tr><td></td><td><a href='/AVANTI'><button style='font-size:24px; width:100px; height:80px;'>SU</button></a></td><td></td></tr>");
  // Riga 2: Sinistra - Stop - Destra
  client.println("<tr>");
  client.println("<td><a href='/SINISTRA'><button style='font-size:24px; width:100px; height:80px;'>SX</button></a></td>");
  client.println("<td><a href='/STOP'><button style='font-size:24px; width:100px; height:80px; background-color:red; color:white;'>STOP</button></a></td>");
  client.println("<td><a href='/DESTRA'><button style='font-size:24px; width:100px; height:80px;'>DX</button></a></td>");
  client.println("</tr>");
  // Riga 3: Indietro
  client.println("<tr><td></td><td><a href='/INDIETRO'><button style='font-size:24px; width:100px; height:80px;'>GIU</button></a></td><td></td></tr>");
  client.println("</table>");
 
  client.println("</body>");
  client.println("</html>");
 
  delay(1);
}
