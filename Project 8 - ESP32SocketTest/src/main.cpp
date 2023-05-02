#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>

const char *SSID = "";
const char *PASSWORD = "";
const int WIFI_TIME_OUT = 2000;

const char *toggle_led_msg = "toggleLED";
const char *get_led_msg = "getLEDState";

const int LED_PIN = 14;

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int led_state = 0;

void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length) {

  IPAddress ip;
  switch (type)
  {
  case WStype_CONNECTED:
    ip = webSocket.remoteIP(client_num);
    Serial.printf("[%u] Connection from: ", client_num);
    Serial.println(ip.toString());
    break;

  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected! \n", client_num);
    break;

  case WStype_TEXT:
    Serial.printf("[%u] Recived text: %s\n", client_num, payload);

    if (strcmp((char *)payload, "toggleLED") == 0)
    {
      led_state = led_state ? 0 : 1;
      digitalWrite(LED_PIN, led_state);
    }
    else if (strcmp((char *)payload, "getLEDState") == 0)
    {
      sprintf(msg_buf, "%d", led_state);
      Serial.printf("Sending to [%d]: %s\n ", client_num, msg_buf);
      webSocket.sendTXT(client_num, msg_buf);
    }
    else
    {
      Serial.println("[%u] Message not recognized!");
    }
    break;

  default:
    break;
  }
}

void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote = request->client()->remoteIP();
  Serial.println("[" + remote.toString() + "] HTTP GET Request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

void onIndexCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote = request->client()->remoteIP();
  Serial.println("[" + remote.toString() + "] HTTP GET Request of " + request->url());
  request->send(SPIFFS, "/styles.css", "text/css");
}

void onIndexScriptRequest(AsyncWebServerRequest *request) {
  IPAddress remote = request->client()->remoteIP();
  Serial.println("[" + remote.toString() + "] HTTP GET Request of " + request->url());
  request->send(SPIFFS, "/script.js", "application/javascript");
}


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);

  if (!SPIFFS.begin()) {
    Serial.println("Error mouting SPIFFS");
  } else {
    Serial.println("Connecting to WiFi");
    Serial.println(SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    unsigned long startAttemp = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemp < WIFI_TIME_OUT) {
      Serial.print(".");
      delay(100);
    }

    if(WiFi.status() != WL_CONNECTED){
      Serial.println("Failed to connect to WiFi!");
    } else {
      Serial.println();
      Serial.println("Connected!");
      Serial.println(WiFi.softAPIP());
      server.begin();

      server.on("/", HTTP_GET, onIndexRequest);
      server.on("/styles.css", HTTP_GET, onIndexCSSRequest);
      server.on("/script.js", HTTP_GET, onIndexScriptRequest);

      server.begin();
      webSocket.begin();
      webSocket.onEvent(onWebSocketEvent);
    }



  } 
}

void loop() {
  webSocket.loop();
}