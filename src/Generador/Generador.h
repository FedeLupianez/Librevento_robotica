#include <WiFiClientSecure.h>
#define Generador_h
#ifdef Generador_h
#include <HTTPClient.h>
#include <WiFi.h>

// Función para configurar la macAddress en la base de datos
void setup_macaddress(HTTPClient& http_client, WiFiClientSecure& wifi_client, const char* user_email);
void upload_medition(HTTPClient& http_client, WiFiClientSecure& wifi_client, float voltage);
#endif
