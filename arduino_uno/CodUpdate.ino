#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DHT.h"

// Enter network credentials:
const char* ssid     = "WIFI12";
const char* password = "cracan12";

// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbzgOML7WGiFKT3s7j1-pnrl0iNqPoUZoAc0JCu3lT_mfeDE_5yDnVxn45JsIuZCoKo_AA";

// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Foaie1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

// Declare variables that will be published to Google Sheets
int value0 = 0;
int value1 = 0;
int value2 = 0;


#define DHT_PIN 2
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {

  Serial.begin(9600);        
  delay(10);
  Serial.println('\n');
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  
  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
       flag = true;
       Serial.println("Connected");
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
}


// --- ESP8266 RECEIVER CODE (Replace your loop with this) ---

void loop() {
  // 1. Check if Arduino sent any data
  if (Serial.available() > 0) {
    
    // Read the line sent by Arduino (Format: "temp,humidity")
    String dataFromUno = Serial.readStringUntil('\n');
    dataFromUno.trim(); // Remove whitespace

    // Simple parsing: split by comma
    int commaIndex = dataFromUno.indexOf(',');
    if (commaIndex > 0) {
      String tempStr = dataFromUno.substring(0, commaIndex);
      String humStr = dataFromUno.substring(commaIndex + 1);
      value0++;

      // 2. Prepare the Google Sheets Payload
      // Note: We use the variables from your original code setup
      // Payload format: {"command": "insert_row", "sheet_name": "Foaie1", "values": "25,60"}
      
      payload = payload_base + "\"" + value0 + "," + tempStr + "," + humStr + "\"}";
      
      // 3. Connection Handling
      static bool flag = false;
      if (!flag){
        client = new HTTPSRedirect(httpsPort);
        client->setInsecure();
        flag = true;
        client->setPrintResponseBody(true);
        client->setContentTypeHeader("application/json");
      }
      
      if (client != nullptr){
        if (!client->connected()){
          client->connect(host, httpsPort);
        }
      }

      // 4. Publish to Google Sheets
      Serial.println("Publishing received data...");
      if(client->POST(url, host, payload)){ 
        Serial.println("Success!");
      }
      else{
        Serial.println("Error while connecting");
      }
    }
  }
  
  // No massive delay needed here, the Arduino controls the timing
  delay(10000); 
}
