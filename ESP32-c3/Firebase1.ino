#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include <DHT.h>

// ---------------- WIFI ----------------
#define WIFI_SSID "-"  //Nume wifi
#define WIFI_PASSWORD "-" //parola

// ---------------- FIREBASE ----------------
#define Web_API_KEY "-"
#define DATABASE_URL "-"
#define USER_EMAIL "-"
#define USER_PASS "-"

// ---------------- DHT SENSOR ----------------
#define DHTPIN 4       // Pinul DHT
#define DHTTYPE DHT11   // Tipul senzorului DHT
DHT dht(DHTPIN, DHTTYPE); //Initializare

// ---------------- Firebase ----------------
void processData(AsyncResult &aResult);
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// ---------------- Timer ----------------
unsigned long lastSendTime = 0; 
const unsigned long sendInterval = 5000; // 5 secunde


void setup() {
  Serial.begin(115200);
  
  // ---------------- WiFi ----------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //Conectare widi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Wi-Fi connected, IP: " + WiFi.localIP().toString());

  // ---------------- DHT ----------------
  dht.begin();

  // ---------------- SSL ----------------
  ssl_client.setInsecure();
  ssl_client.setConnectionTimeout(1000);
  ssl_client.setHandshakeTimeout(5);

  // ---------------- Firebase ----------------
  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}

void loop() {
  app.loop();

  if (app.ready()) {
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval) { //Daca timpul dintre o masurare depaseste 5 secunde are loc o masurare cronometrul se reseteaza
      lastSendTime = currentTime;
      // ---------------- Citire DHT ----------------
      float temp = dht.readTemperature(); //Temperatura
      float hum = dht.readHumidity(); //Umiditatea

      if (!isnan(temp) && !isnan(hum)) {
        Database.set<float>(aClient, "/sensor/temperature", temp, processData, "RTDB_DHT_Temp"); //Calea de transmitere temperatura
        Database.set<float>(aClient, "/sensor/humidity", hum, processData, "RTDB_DHT_Hum"); //Calea de transmitere umiditatea
        Serial.printf("DHT - Temp: %.2f, Hum: %.2f\n", temp, hum);
      } else {
        Serial.println("Nu sa pututu citi date de la dht11!");
      }
    }
  }
}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult()) return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available())
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
}
