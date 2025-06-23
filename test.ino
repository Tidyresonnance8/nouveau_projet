#include <WiFi.h>

const char* ssid = "TonSSID";
const char* password = "TonMotDePasse";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connexion");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté à : " + WiFi.localIP().toString());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<h1>Bonjour depuis ESP32-WROOM-32U</h1>");
    client.println();
    client.stop();
  }
}