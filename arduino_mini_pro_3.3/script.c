#include "DHT.h"

#define DHTPIN 2 // Digital pin connected to the DHT sensor 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  // Initialize device.
  dht.begin();
}

// the loop function runs over and over again forever
void loop() {
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %");
    Serial.print(" | Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    delay(2000); //Delay 2 sec.
}