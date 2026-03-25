#include <ESP8266WiFi.h>

const char* ssid = "IL_TUO_NOME_WIFI";
const char* password = "LA_TUA_PASSWORD_WIFI";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.println();
  Serial.print("Connessione alla rete: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi Connesso con successo!");
  Serial.print("Il tuo indirizzo IP è: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
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

  // Nuova pagina web con layout a croce
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1.0'></head>"); // Adatta allo schermo del telefono
  client.println("<body style='text-align:center; font-family:Arial;'>");
  client.println("<h1>Telecomando Rover</h1>");
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
