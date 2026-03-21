#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

// Configurare DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configurare MQ-135
#define MQ_PIN A0

// Configurare Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac, IPAddress(192, 168, 1, 177));
  }
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          
          // Citire senzori
          float h = dht.readHumidity();
          float t = dht.readTemperature();
          int airQuality = analogRead(MQ_PIN); // Valoare intre 0 si 1023

          // Determinare stare aer (aproximativa)
          String airStatus;
          String airColor;
          if(airQuality < 200) { airStatus = "Excelent"; airColor = "#28a745"; }
          else if(airQuality < 400) { airStatus = "Moderat"; airColor = "#ffc107"; }
          else { airStatus = "Poluat / Fum"; airColor = "#dc3545"; }

          // Header HTTP
          client.println(F("HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nRefresh: 10\n"));

          // HTML & CSS
          client.println(F("<!DOCTYPE HTML><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"));
          client.println(F("<style>body{font-family:sans-serif; text-align:center; background:#eceff1; margin-top:50px;}"));
          client.println(F(".card{display:inline-block; background:white; padding:30px; border-radius:15px; box-shadow:0 4px 8px rgba(0,0,0,0.1);}"));
          client.println(F(".val{font-size:1.8em; font-weight:bold;} .label{color:#555;}</style></head><body>"));
          
          client.println(F("<div class='card'>"));
          client.println(F("<h2>Statie Monitorizare Mediu</h2><hr>"));
          
          // Afisare DHT11
          client.print(F("<p class='label'>Temp: <span class='val' style='color:#f44336;'>"));
          client.print(t, 1); client.println(F(" &deg;C</span></p>"));
          client.print(F("<p class='label'>Umid: <span class='val' style='color:#2196f3;'>"));
          client.print(h, 0); client.println(F(" %</span></p>"));
          
          // Afisare MQ-135
          client.print(F("<p class='label'>Calitate Aer: <br><span class='val' style='color:"));
          client.print(airColor); client.print(F(";'>"));
          client.print(airStatus); client.print(F(" ("));
          client.print(airQuality); client.println(F(")</span></p>"));
          
          client.println(F("<p style='font-size:0.8em; color:grey;'>Actualizat la 10 sec.</p>"));
          client.println(F("</div></body></html>"));
          break;
        }
        if (c == '\n') currentLineIsBlank = true;
        else if (c != '\r') currentLineIsBlank = false;
      }
    }
    delay(10);
    client.stop();
  }
}