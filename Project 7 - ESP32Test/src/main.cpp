#include <Arduino.h>
#include <WiFi.h>

const char *SSID = "WIFINAME";
const char *PASSWORD = "WIFIPASSWORD";
const int WIFI_TIME_OUT = 2000;
const int LED_PIN = 22;

String output22State = "off";

WiFiServer server(80);
String header;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

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
    Serial.println(WiFi.localIP());
    server.begin();
  }


}

void loop() {
  WiFiClient client = server.available();
  
  if(client) {
    Serial.println("New client connected!");
    String line = "";

    while (client.connected())
    {
      if(client.available()){
        char c = client.read();
        Serial.write(c);
        header += c;

        if(c == '\n'){

          if(line.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
          
          
            if(header.indexOf("GET /22/on") >= 0){
              output22State = "on";
              digitalWrite(LED_PIN, HIGH);
            } else if(header.indexOf("GET /22/off") >= 0){
              output22State = "off";
              digitalWrite(LED_PIN, LOW);
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<body><h1>ESP32 Diode NapicuTest</h1>");
            
            if (output22State=="off") {
              client.println("<p><a href=\"/22/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/22/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            client.println("</body></html>");
            
            client.println();
            break;
          } else {
            line = "";
          }
        } else if (c != '\r'){
          line += c;
        }
      }
    }

    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    
  }
}