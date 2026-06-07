
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// WiFi credentials imported from secrets.h
#include "secrets.h"

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10          /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

// Create instances
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// Variables for timing
long previous_time = 0;

void setupMQTT() {
  mqttClient.setServer(mqtt_broker, mqtt_port);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  int attempts = 0;
  while (!mqttClient.connected() && attempts < 10) {
    Serial.println("Reconnecting to MQTT Broker attempt: "+ String(attempts));
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker.");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      attempts++;
    }
  }

}

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

void mqttHiveSendData() {
    if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();
  
  long now = millis();
    
    // Convert the value to a string
    String value_str = String(now);
 
    // Publish the sensor value to the MQTT topic
    mqttClient.publish(topic_publish_ir, value_str.c_str());
    Serial.print("Value: ");
    Serial.println(value_str);
    delay(2000);
 }



void setup() {
  Serial.begin(115200);
  delay(5000);  //Take some time to open up the Serial Monitor

    //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

//Print the wakeup reason for ESP32
  print_wakeup_reason();

  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  // Try to connect for up to 20 seconds
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    Serial.println("try again in 2 seconds");
    delay(2000);
    attempts++;
    Serial.print("attempt: "+ String(attempts));
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize secure WiFiClient
    wifiClient.setInsecure(); // Use this only for testing, it allows connecting without a root certificate
    
    setupMQTT();
    mqttHiveSendData();

  } else {
    Serial.println("\nFailed to connect to WiFi within timeout.");
  }
  
  
  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 10 seconds
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.println("Going to sleep now");
  Serial.flush();

  // Turn off WiFi to save power before entering deep sleep
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}
void loop() { }
