Hello Smart Hive touch README.mdtouch README.md
# Arduino UNO
## script
    - DHT11
    - MQ153
    - ESP8266
# Arduino mini pro 3.3v
## script
    - DHT11
    - MQ153
    - ESP8266
---------------------------------------------
# ESP32-C3 
## Descriere
* citește date de la senzori:
  *  BME280 (temp, presiune)
  *  DHT11 (umiditate)
  *  MQ (gaz)
* transmite date prin WiFi la fiecare **30 minute**
* intră în **deep sleep** între transmisii
---
## Alimentare
* 2× baterii Li-Ion 3.7V (ex: 18650) în **paralel**
*  Tensiune: **3.7V**
*  Capacitate totală: **≈ 6000 mAh**
* Necesită:
  * modul **BMS (protecție)**
  * convertor **step-down la 3.3V**
---
## Consum estimativ
###  Mod activ (~10 sec) !!Cu MQ pornit timpul de pornire 2-3 minute
* ESP32 + WiFi: ~120 mA
* Senzori: ~3 mA
* MQ: ~180 mA
* Total: **~300 mA**
Consum:
300 mA × 10 s ≈ **0.83 mAh**
---
### Deep sleep (~30 min)
* ESP32: ~0.02 mA
* MQ:  ~180 mA (dacă rămâne pornit)
Fără oprire MQ: **~24.8 mAh / ciclu**
Cu MQ oprit: **~0.01 mAh / ciclu**
---
## Problemă
Senzorul MQ consumă foarte mult și trebuie **oprit în deep sleep**.
---
Pentru a opri senzorul MQ putem folosi:
* Folosește **MOSFET** pentru a opri MQ când ESP32 intră în sleep
* !!!MQ poate avea valori eronate la pornire dupa deep sleep, are nevoie de o perioada de 1-2 minute pentru incalzire!!
* Nu se pot garanta valori precise nici dupa 1-2 minute de incalzire 
---
## Autonomie estimată
* Fără optimizare: ~3 zile
* Cu optimizare(MQ oprit): ~5 luni
---

## Concluzie
* ESP32-C3 este foarte eficient în deep sleep
* WiFi consumă mult, dar pe termen scurt
* Autonomia depinde de oprirea senzorului MQ

### Deep Sleep,resurese
* https://github.com/junk495/Esp32C3_DeepSleep
* https://espressif.github.io/esp32-c3-book-en/chapter_12/12.2/12.2.3.html

### Firebase,resurse
* https://randomnerdtutorials.com/esp32-firebase-realtime-database/

### Librarie BMP280, resurse 
* https://github.com/Erriez/ErriezBMX280/tree/master?tab=readme-ov-file

# Montaj Panou Solar
* 1. Captarea (Panoul Solar)
Un panou solar monocristalin de 6V cu o putere de minim 5W.

2. Conversia Inteligentă

Putem folosi TP4056,dar vor fi pierderi de energie o optiune mai buna MPPT

Ce ai nevoie exact: Un modul de încărcare CN3791. Atenție maximă când îl comanzi: trebuie să alegi versiunea calibrată fix pentru tensiunea panoului tău (adică versiunea de 6V).

Ce face: Funcționează în comutație (MPPT). Stoarce matematic puterea maximă din panou, indiferent de cât de slab este soarele, și o bagă în baterie cu eficiență de 90%.

Stocarea (Acumulatorul)

Un suport de baterie 18650 cu fire groase.

Distribuția (Regulatoarele de tensiune)

Bateria va avea o tensiune variabilă între 3.0V (goală) și 4.2V (plină). Niciuna dintre componentele tale nu folosește această plajă.
Trebuie să creezi două magistrale de alimentare separate.
Magistrala Logicii (pentru ESP32-C3 și senzori): * Acestea merg strict la 3.3V. Dacă le dai 4.2V din bateria plină, le arzi.

Ce ai nevoie: Un modul regulator LDO (Low Dropout) de 3.3V, capabil de minim 500mA. Exemple: modul cu cip HT7333, AP2112K sau XC6220. Consumă extrem de puțin în repaus.

Magistrala de Putere (pentru modulul celular SIM7000G):

Modulul celular are nevoie de o tensiune stabilă în jur de 3.8V - 4.0V (verifică datasheet-ul exact al plăcii pe care o cumperi) și trage vârfuri de 2A.

Ce ai nevoie: Un modul DC-DC Step-Down (dacă GSM-ul cere 3.8V) capabil de 2A reali. Ieșirea acestuia merge doar la modulul GSM.

Obligatoriu: Un condensator electrolitic de 2200µF / 10V. Se lipește fizic direct pe pinii de alimentare ai modulului GSM pentru a acționa ca un rezervor de șoc în secunda în care antena emite.

Schema logică de conexiune a acestor piese:

[Panou Solar] ---> [CN3791] ---> [Baterie 18650]
De la pinii bateriei (sau de la ieșirea de sarcină a CN3791, dacă are), tragi fire în două direcții paralele:
Ruta 1 ---> [Regulator LDO 3.3V] ---> [Alimentare ESP32 și Senzori]
Ruta 2 ---> [Regulator DC-DC] ---> [Alimentare Modul GSM + Condensator]


# Arduino UNO with W5100
## Problemă,Soluție
- Conflict de pini,"Shield-ul Ethernet folosește pinii 10, 11, 12 și 13 pentru comunicarea SPI. Nu îi folosi pentru alte componente (senzori, LED-uri)."
- Pinul 4 (Card SD),"Multe shield-uri W5100 au și un slot de card SD. Acesta folosește pinul 4. Dacă nu folosești cardul, e bine să setezi pinul 4 pe OUTPUT și HIGH în setup pentru a evita interferențele."
- Adresa IP,Verifică dacă adresa IP pe care o primește Arduino este în aceeași clasă cu routerul tău (ex: ambele să înceapă cu 192.168.1.x).

## Sfaturi Utile:
- Căldură: Chip-ul W5100 tinde să se încălzească destul de tare în timpul funcționării. Este normal, dar asigură-te că are puțin flux de aer.
- Alimentare: Dacă ai probleme de conexiune instabilă, încearcă să alimentezi Arduino cu o sursă externă de 9V/1A, deoarece shield-ul consumă destul de mult curent (~150-200mA).

## Cum funcționează Cod Web Server HTML
- Header-ul HTTP: Este "protocolul de salut". Browserul are nevoie de HTTP/1.1 200 OK ca să știe că totul e în regulă.
- client.println(): Această funcție trimite textul direct către browser. Practic, "desenezi" pagina linie cu linie.
- Refresh automat: Am adăugat linia client.println("Refresh: 5");. Aceasta îi spune browserului să reîncarce pagina singur la fiecare 5 secunde, util dacă vrei să monitorizezi senzori în timp real.

Consumul de memorie: Folosim client.print pentru bucăți mici de text. Dacă vrei să salvezi și mai multă memorie RAM, poți folosi macro-ul F(), de exemplu: client.print(F("<html>"));. Acesta stochează textul în memoria Flash (unde e codul), nu în RAM.

## Conectarea Hardware (DHT11)
Senzorul DHT11 are de obicei 3 sau 4 pini. Dacă folosești un modul cu 3 pini (deja montat pe o plăcuță mică), conexiunile sunt:
- VCC (sau +): Conectează la 5V pe Arduino.
- GND (sau -): Conectează la GND pe Arduino.
- DATA (sau Out): Conectează la pinul digital 2 pe Arduino.
- Notă: Dacă ai senzorul simplu (cu 4 pini), trebuie să pui o rezistență de $4.7\text{ k}\Omega$ sau $10\text{ k}\Omega$ între pinii VCC și DATA pentru "pull-up".

## Conectarea Hardware (MQ-135)
Senzorul MQ-135 are de obicei 4 pini. Îl vom folosi pe cel Analogic (AO) pentru a obține o valoare variabilă a calității aerului.
- VCC: Conectează la 5V pe Arduino.
- GND: Conectează la GND pe Arduino.
- AO (Analog Out): Conectează la pinul A0 pe Arduino.
- DO (Digital Out): Nu îl folosim acum, acesta trimite doar 0 sau 1 în funcție de un prag setat manual din potențiometru.

Notă importantă: Senzorii din seria MQ au nevoie de o perioadă de "încălzire" (pre-heating). La prima utilizare, lasă-l pornit aproximativ 24 de ore. În utilizarea zilnică, valorile devin stabile după 2-3 minute de la alimentare.

### Detalii despre MQ-135:
- Valoarea Analogică: analogRead(A0) îți returnează un număr între 0 și 1023.
- Sub 200: Aer curat.
- 200 - 400: Calitate medie (poate fi mult CO2 sau mirosuri).
- Peste 400: Aer poluat, fum sau gaze toxice.

Calibrare: Pragurile de mai sus sunt orientative. Fiecare senzor MQ-135 diferă puțin. Poți ajusta valorile din cod după ce vezi ce cifre raportează senzorul tău într-un mediu cu aer curat.

# Arduino UNO with ESP8266

