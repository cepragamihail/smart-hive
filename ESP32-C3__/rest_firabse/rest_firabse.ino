#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// Wi-Fi credentials
const char* ssid = "XXX";
const char* password = "XXXX";

// Your Firebase REST URL (Crucial: Must end with .json)
// Format: https://[PROJECT-ID].firebaseio.com/[PATH].json
// If your database has security rules, append your token: ?auth=YOUR_TOKEN
const char* firebase_url = "https://[YOUR-PROJECT][.firebaseio.com/data.json?auth=YOUR_EXTRACTED_ID_TOKEN]";


void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    
    // 1. Create a secure client
    WiFiClientSecure client;
    client.setInsecure(); // Tells the ESP32 to trust Firebase without uploading a massive SSL certificate file
    
    HTTPClient http;
    
    // 2. Initialize the connection
    http.begin(client, firebase_url);
    http.addHeader("Content-Type", "application/json");
    
    // 3. Prepare your JSON payload
    // This creates: {"temperature": 24.5, "uptime_seconds": 120}
    String jsonPayload = "{\"temperature\":" + String(random(20, 30)) + 
                         ",\"uptime_seconds\":" + String(millis() / 1000) + "}";
    
    Serial.println("\nSending payload: " + jsonPayload);
    
    // 4. Send the HTTP Request
    // Use http.PUT(jsonPayload) to overwrite the entire node
    // Or use http.sendRequest("PATCH", jsonPayload) to only update specific fields
    int httpResponseCode = http.PUT(jsonPayload); 
    
    // 5. Handle the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Firebase response: " + response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    // 6. Close connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  
  // Wait 10 seconds before sending next payload
  delay(10000); 
}
