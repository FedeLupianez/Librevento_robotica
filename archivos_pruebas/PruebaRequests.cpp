// Archivo principal del proyecto
// Acá vamos a controlar todos los tipos,
// structs y manejar el loop principal, como
// un archivo .ino común
#include <WiFi.h>
#include <esp_http_client.h>

const char* ssid = "ejemplo";
const char* password = "123";
// Urls de pruebas
const char* getIdUrl = "http://httpbin.org/get";
const char* postUrl = "https://httpbin.org/post";
// Simula un json, que es el formado como se manda la info del
// aerogenerador para que se almacene en la base de datos
const char* exampleData = "{\"nombre\":\"fede\", \"edad\":25}";


esp_err_t event_handler(esp_http_client_event_t* event) {
  // En esta función voy a manejar el event que me llega según su tipo
  if (event->event_id != HTTP_EVENT_ON_DATA){
    // Si es distinto a un event que tiene data, retorna un fail
    return ESP_FAIL;
  }
  Serial.println("Datos recibidos : ");
  // Con (char*) se transforma lo que viene en string
  // es como en python hacer str(numero). Y se imprime
  Serial.write((char*)event->data, event->data_len);
  return ESP_OK;
}

void setup() {
  Serial.begin(115200);
  // trato de conectar a internet
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ...");
  }
  Serial.println("Placa conectada a la ssid");
}

void loop() {

  if (Serial.available() > 0){
    int option = Serial.read();

    // Opción para realizar una request GET
    if (option == 0){
      // Configuro el client
      esp_http_client_config_t config = {
        .url = getIdUrl, // Url a la que va a hacer la petición 
        .event_handler = event_handler, // Función que va a ejecutar al tener la respuesta
      };

      esp_http_client_handle_t client = esp_http_client_init(&config);
      // Ejecuto el client
      esp_err_t response = esp_http_client_perform(client);

      if (response == ESP_OK){
        Serial.printf("Request exitosa %d\n", esp_http_client_get_status_code(client));
      }else{
        // Imprimo el nombre del error
        Serial.printf("Request fallida %d\n", esp_err_to_name(response));
      }

      // Limpio el client
      esp_http_client_cleanup(client);
    }


    if (option == 1){
      esp_http_client_config_t config = {
        .url = postUrl,
        .event_handler = event_handler,
        //.cert_pem = NULL, // Esto hace que se saltee la certificación SSL, solo para testear en caso de ser necesario
      };
      // Creo un cliente http 
      esp_http_client_handle_t client = esp_http_client_init(&config); // Esta función es la que crea un client

      esp_http_client_set_method(client, HTTP_METHOD_POST); // Le configuro el metodo como Post
      esp_http_client_set_header(client, "content-type", "application/json"); // Le digo que el contenido que va a mandar es tipo json
      esp_http_client_set_post_field(client, exampleData, strlen(exampleData)); // Le pongo la info que va a mandar

      esp_err_t response = esp_http_client_perform(client);
      if (response == ESP_OK){
        Serial.printf("Request exitosa %d\n", esp_http_client_get_status_code(client));
      }else{
        Serial.printf("Request Fallida %d\n", esp_err_to_name(response));
      }
      // Limpio el client
      esp_http_client_cleanup(client);
    }
  }
}