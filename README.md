# badgeuse_esp8266

Ceci est le code pour la carte esp8266 qui lit les badges rfid et contrôle la gachette de la porte.  
Elle communique avec un script sur un serveur qui va gérer les actions à réaliser à la lecture d'un badge.  
Pour communiquer avec le serveur, elle utilise le protocole MQTT.

## Foctionnalités
* ✔ Se connecter à un réseau wifi
* ✔ Lire l'UID des badges RFID 13,56 MHz avec un module MFRC522
* ✔ Communiquer avec un serveur MQTT (publisher et subscriber)
* ❌ Commander la gachette d'une porte
* ❌ Indiquer l'état avec deux leds
* (❌ Mettre en place du cryptage pour l'échange avec le serveur)

## Installation
Vous avez besoin des cartes ajoutées par "ESP8266 core for Arduino".  
[Pour l'installer, renseignez vous sur le github](github.com/esp8266/Arduino).  
Vous devez compiler le code avec l'[IDE Arduino][ard] en choisissant la carte esp qui correspond à la votre.  


[ard]:https://www.arduino.cc/en/Main/Software
