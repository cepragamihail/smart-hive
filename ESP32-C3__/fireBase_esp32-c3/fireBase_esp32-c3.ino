
#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "secrets.h"

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10          /* Time ESP32 will go to sleep (in seconds) */

// User function
void processData(AsyncResult &aResult);

// Authentication
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

// Firebase components
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;


// Timer variables for sending data every 10 seconds
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds in milliseconds

// Variables to send to the database
int intValue = 0;
float floatValue = 0.01;
String stringValue = "";

volatile bool stringSent = false;
volatile bool intSent = false;
volatile bool floatSent = false;


RTC_DATA_ATTR int bootCount = 0;


/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();

  // 1. Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    ssl_client.setInsecure();
    ssl_client.setConnectionTimeout(1000);
    ssl_client.setHandshakeTimeout(5);
    
    // 2. Initialize Firebase
    initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
    app.getApp<RealtimeDatabase>(Database);
    Database.url(DATABASE_URL);

    // 3. Wait for authentication to become ready (with a timeout of 10s)
    Serial.println("Waiting for Firebase App to become ready...");
    unsigned long authStart = millis();
    while (!app.ready() && millis() - authStart < 10000) {
      app.loop();
      delay(50); // Yield control to ESP32 RTOS
    }

    if (app.ready()) {
      Serial.println("Firebase ready! Queueing data...");
      
      // Reset transfer completion flags
      stringSent = false;
      intSent = false;
      floatSent = false;

      // Queue database writes
      unsigned long currentTime = millis();
      stringValue = "value_" + String(currentTime);
      Database.set<String>(aClient, "/test/string", stringValue, processData, "RTDB_Send_String");
      Database.set<int>(aClient, "/test/int", intValue, processData, "RTDB_Send_Int");
      
      floatValue = 0.01 + random(0, 100);
      Database.set<float>(aClient, "/test/float", floatValue, processData, "RTDB_Send_Float");

      // 4. Poll app.loop() until all data tasks have been successfully processed (with a timeout of 8s)
      unsigned long sendStart = millis();
      while (!(stringSent && intSent && floatSent) && millis() - sendStart < 8000) {
        app.loop();
        delay(50);
      }

      if (stringSent && intSent && floatSent) {
        Serial.println("All data sent successfully!");
      } else {
        Serial.println("Warning: Timeout reached. Some data might not have been sent.");
      }

    } else {
      Serial.println("Firebase Auth failed or timed out.");
    }

  } else {
    Serial.println("\nFailed to connect to WiFi within timeout.");
  }

  // 5. Configure and go to deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep now");
  Serial.flush();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_deep_sleep_start();
}


void loop(){}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult())
    return;

  // Log events/errors/payloads
  if (aResult.isError()) {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }
  if (aResult.available()) {
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
  }

  // Set flags when the specific tasks complete
  if (aResult.uid() == "RTDB_Send_String") stringSent = true;
  if (aResult.uid() == "RTDB_Send_Int") intSent = true;
  if (aResult.uid() == "RTDB_Send_Float") floatSent = true;
}
