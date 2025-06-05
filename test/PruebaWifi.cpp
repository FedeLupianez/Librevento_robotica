// Antes que todo, instalar las librerias en arduino IDE,
// copiar el código y pegarlo en un sketch de arduino nuevo,
// ya que si el nombre del archivo y la carpeta que lo contiene 
// no son los mismos, arduino no lo puede compilar
#include <WiFi.h>
#include <WebServer.h>

// Cambia esto por tus credenciales WiFi
const char* ssid = "TU_SSID";
const char* password = "TU_CONTRASEÑA";

// Crear el servidor en el puerto 80
WebServer server(80);

const int ledPin = 2;  // Puedes cambiar esto al pin que uses

// Página HTML básica
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Control LED</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    button { padding: 20px; font-size: 24px; margin: 20px; }
  </style>
</head>
<body>
  <h1>Control de LED con ESP32</h1>
  <button onclick="fetch('/on')">Encender</button>
  <button onclick="fetch('/off')">Apagar</button>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED encendido");
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED apagado");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configuración de rutas
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}