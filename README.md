# badgeuse_esp8266

Ceci est le code pour la carte esp8266 qui lit les badges rfid et contrôle la gachette de la porte.  
Elle communique avec un script sur un serveur qui va gérer les actions à réaliser à la lecture d'un badge.  
Pour communiquer avec le serveur, elle utilise le protocole MQTT.

## Foctionnalités
* Se connecter à un réseau wifi
* Lire l'UID des badges RFID 13,56 MHz avec un module MFRC522
* Communiquer avec un serveur MQTT (publisher et subscriber)
* Commander la gachette d'une porte
* Indiquer l'état avec deux leds

## Installation
Il faut compiler le code avec l'[IDE ARduino][ard] en choisissant la carte qui correspond.  
Si vous n'avez pas les cartes


[ard]:https://www.arduino.cc/en/Main/Software