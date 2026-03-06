# Problema de Programare: Conflict de Port și Sincronizare

Acesta este un conflict între portul selectat în Arduino IDE și adresa reală a adaptorului tău USB-to-Serial. Spre deosebire de Arduino Uno, ATmega Pro Mini nu are un port USB integrat și necesită un adaptor extern (precum FTDI sau CH340), care de obicei apare sub o altă denumire în sistemele Linux. 

Iată pașii pentru a remedia eroarea:

### 1. Identifică portul corect 
Mesajul tău indică faptul că IDE-ul caută `/dev/ttyACM0`, dar adaptoarele externe sunt aproape întotdeauna recunoscute ca `/dev/ttyUSB0`. 

* Deconectează adaptorul Pro Mini.
* Deschide un terminal și rulează comanda: `ls /dev/ttyUSB*`.
* Conectează adaptorul și rulează comanda din nou. Numele care apare nou (ex: `/dev/ttyUSB0`) este cel pe care trebuie să îl selectezi în **Tools > Port** în Arduino IDE. 

### 2. Verifică permisiunile (Linux)
Dacă portul apare dar primești "Permission denied", trebuie să adaugi utilizatorul tău în grupul `dialout`: 

* Rulează în terminal: `sudo usermod -a -G dialout $USER`
* **Important**: Trebuie să închizi sesiunea (Log out) și să te reconectezi pentru ca schimbarea să aibă efect. 

### 3. Configurații esențiale pentru Pro Mini
Asigură-te că setările din meniul **Tools** sunt corecte pentru placa ta specifică: 

* **Board**: "Arduino Pro or Pro Mini"
* **Processor**: Verifică dacă ai varianta de 5V (16 MHz) sau 3.3V (8 MHz). Dacă selectezi frecvența greșită, upload-ul va eșua cu erori de sincronizare (`stk500_getsync`). 

### 4. Sincronizarea manuală (Reset)
Dacă adaptorul tău nu are pinul DTR conectat la pinul Reset al Pro Mini, placa nu se va reseta automat pentru programare. 

* Apasă butonul **Reset** de pe Pro Mini exact în momentul în care Arduino IDE afișează mesajul "Uploading..." în bara de stare. 

> **Întrebare**: Ai conectat pinul DTR al adaptorului la pinul DTR/GRN al plăcii Pro Mini pentru auto-reset?

---
### Referințe (Rezolvare probleme port)

[1] [YouTube](https://www.youtube.com/watch?v=tKCiNepc6_4)  
[2] [Arduino Docs](https://docs.arduino.cc/retired/getting-started-guides/ArduinoProMini/)  
[3] [Raspberry Pi Forums](https://forums.raspberrypi.com/viewtopic.php?t=184852)  
[4] [Arduino Forum 1](https://forum.arduino.cc/t/solved-bymiracle-uploads-failing-avrdude-stk500v2_receivemessage-timeout/299416#:~:text=Thank%20you%20for%20your%20comments.%20The%20ports,get%20quite%20used%20to%20swoping%20between%20them.)

---

## Conectarea și Configurarea Adaptorului FTDI

### 1. Configurarea adaptorului FTDI
Majoritatea adaptoarelor FTDI au un jumper sau o punte de cositor care permite selectarea între 5V și 3.3V.

* **Verifică jumperul**: Mută-l în poziția 3.3V înainte de a face orice conexiune.
* Dacă adaptorul tău nu are jumper, verifică pe spate dacă există pad-uri de lipit pentru selectarea tensiunii. 

### 2. Schema de cablare (Pin la Pin)
Dacă adaptorul tău are un header cu 6 pini, acesta se potrivește de obicei direct în mufa Pro Mini. Dacă folosești fire jumper, iată corespondența corectă:

| Pin FTDI | Pin Pro Mini | Explicație |
|---|---|---|
| GND | GND (sau BLK) | Masa comună. |
| VCC | VCC | Alimentarea (asigură-te că e 3.3V). |
| TX | RX (sau RXI) | Transmisia de date (se leagă în cruce). |
| RX | TX (sau TXO) | Recepția de date (se leagă în cruce). |
| DTR | DTR (sau GRN/Reset) | Pinul pentru auto-reset (necesar pentru upload). |
| CTS | Neconectat | De obicei se lasă liber sau la GND. |

### 3. Verificarea orientării

* Plăcile originale au marcaje pe margini: BLK (Black/GND) și GRN (Green/DTR).
* Aliniază pinul DTR al adaptorului cu pinul GRN de pe Pro Mini.
* Dacă adaptorul tău are doar 4 pini (fără DTR), va trebui să apeși manual butonul **Reset** de pe Pro Mini exact în momentul în care Arduino IDE afișează "Uploading...". 

### 4. Setări în Arduino IDE
După conectare, mergi la meniul **Tools** și selectează:

* **Board**: Arduino Pro or Pro Mini.
* **Processor**: ATmega328P (3.3V, 8 MHz) — **Atenție**: Dacă este setat pe 5V/16MHz, upload-ul va eșua din cauza vitezei de comunicare greșite.
* **Port**: Selectează portul care apare nou la conectare (de obicei `/dev/ttyUSB0` pe Linux sau `COMx` pe Windows). 

---
### Referințe (Conectare și configurare)

[1] [Arduino Forum 2](https://forum.arduino.cc/t/ftdi-serial-adapter-with-3-3v-pro-mini/185554)  
[2] [SparkFun Tutorial](https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v/all)  
[3] [MySensors Forum](https://forum.mysensors.org/topic/410/ftdi-arduino-pro-mini)
