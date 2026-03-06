# Arduino Pro Mini 3.3V

Versiunea Arduino Pro Mini 3.3V este un microcontroler compact bazat pe ATmega328P, care funcționează la o frecvență de 8 MHz. Deoarece nu are un port USB integrat, programarea și alimentarea se realizează prin pinii dedicați. 

Iată descrierea detaliată a pinilor, grupați pe funcții:

## 1. Pini de Alimentare (Power)

* **VCC** (2 pini): Furnizează tensiunea reglată de 3.3V direct către microcontroler. Dacă ai o sursă de 3.3V deja stabilizată, o conectezi aici.
* **GND** (3 pini): Reprezintă masa comună (0V) a circuitului.
* **RAW**: Pinul de intrare pentru tensiune ne-reglată. Poate accepta între 3.4V și 12V. Regulatorul de pe placă va converti această tensiune la 3.3V pentru funcționarea sistemului. 

## 2. Pini Digitali (14 pini: 0–13)

Toți pinii digitali funcționează la o logică de 3.3V. 

* **0 (RX)** și **1 (TX)**: Folosiți pentru comunicația serială TTL (primire și transmitere date).
* **2** și **3** (Întreruperi Externe): Pot fi configurați să declanșeze o acțiune imediată la schimbarea stării semnalului.
* **PWM** (3, 5, 6, 9, 10, 11): Oferă ieșire analogică simulată pe 8 biți prin funcția `analogWrite()`.
* **13 (LED-ul integrat)**: Conectat la un LED de pe placă; când pinul este HIGH, LED-ul se aprinde. 

## 3. Pini Analogici (8 pini: A0–A7)

* **A0 – A3**: Pini de intrare analogică standard situați pe marginea plăcii.
* **A4 (SDA)** și **A5 (SCL)**: Utilizați pentru comunicația I2C. Aceștia se află adesea în interiorul plăcii, nu pe margine.
* **A6** și **A7**: Intrări analogice suplimentare care pot fi folosite doar ca intrări (nu și ca pini digitali). 

## 4. Interfața de Programare (Header-ul de 6 pini)

Situat pe una dintre laturile scurte, acest rând de pini este aliniat special pentru conectarea unui [adaptor FTDI de 3.3V](https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v/all): 

* **DTR / GRN**: Folosit pentru auto-resetarea plăcii în timpul încărcării codului.
* **TXO / RXI**: Liniile de date seriale pentru comunicarea cu computerul.
* **VCC / GND**: Alimentarea de la adaptor. 

## 5. Pinul RESET

* **RST**: Aduce microcontrolerul la starea inițială dacă este conectat la GND (stare LOW). Este util dacă programul se blochează. 

> **Notă importantă**: Deoarece acesta este modelul de 3.3V, nu trebuie să conectezi niciodată senzori sau module care scot 5V direct la pinii de date, deoarece pot arde placa. 

---

### Referințe

[1] [Adafruit](https://www.adafruit.com/product/2377)  
[2] [Richard Electronics](https://www.richardelectronics.com/blog/projects/arduino/arduino-pro-mini-pinout-datasheet-features-ide-and-simulation)  
[3] [YouTube - Example 1](https://www.youtube.com/watch?v=1m8FIUZE8To&t=138)  
[4] [The Engineering Projects](https://www.theengineeringprojects.com/2018/06/introduction-to-arduino-pro-mini.html)  
[5] [YouTube - Example 2](https://www.youtube.com/watch?v=a8o2MdktACg&t=66)  
[6] [SparkFun - Using the Arduino Pro Mini 3.3V](https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v/all?print=1)  
[7] [Scribd](https://www.scribd.com/document/647698572/Arduino-Pro-Mini-DETAILS)  
[8] [Geeetech Wiki](https://wiki.geeetech.com/index.php/Arduino_Pro_Mini)  
[9] [Arduino Official](https://www.arduino.cc/en/Main/ArduinoBoardProMini)  
[10] [SparkFun - What is an Arduino](https://learn.sparkfun.com/tutorials/what-is-an-arduino/whats-on-the-board#:~:text=PWM%20%288%29:%20You%20may%20have%20noticed%20the,used%20for%20something%20called%20Pulse%2DWidth%20Modulation%20%28PWM%29.)  
[11] [RS Online](https://www.rs-online.com/designspark/what-is-arduino-pro-mini-a-getting-started-guide)  
[12] [Arduino Forum 1](https://forum.arduino.cc/t/arduino-pro-mini-3-3v-xbee-mpu6050-can-i-use-them-simultanoulsly/400902)  
[13] [Ardubotics](https://ardubotics.eu/en/development-boards/1053-pro-mini-33-pro-mini-50-compatible-arduino.html)  
[14] [SparkFun Tutorial Print](https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v/all?print=1)  
[15] [SparkFun Tutorial](https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v/all)  
[16] [SparkFun Datasheet](https://www.verical.com/datasheet/sparkfun-electronics-embedded-system-development-tools-dev-11114-5902681.pdf)  
[17] [Arduino Forum 2](https://forum.arduino.cc/t/running-arduino-pro-mini-5v-16mhz-board-3-3v/336706)  
[18] [Arduino Docs - Pro Mini](https://docs.arduino.cc/retired/getting-started-guides/ArduinoProMini/)
    