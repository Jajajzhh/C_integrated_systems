#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>

// les parametres pour WiFi
const char ssid[20]     = "d206-IOT-AP";
const char password[20] = "d2062019uttlo11";

// les parametres pour WiFiClient
const char host[20] = "192.168.2.40";
const String requete = "GET /michel/ HTTP/1.1\r\n\ 
Host: 192.168.2.40\r\n\ 
Connection: close\r\n\r\n";


// le method pour connecter WiFi
void ConnexionWifi(char *ssid,const char *passphrase) {
  int etatled=0;
  Serial.print("connect to ");
  Serial.println(ssid);
  WiFi.begin(ssid, passphrase);
  delay(100);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    etatled ^= 1;
    digitalWrite(LED,etatled);
  }
  Serial.println("connected");
  digitalWrite(LED,HIGH);
}


// afficher tous les parametres wifi 
void AfficheParametres() {
  byte adrMAC[6];
  IPAddress ip,gw,masque;
  WiFi.macAddress(adrMAC);
  for(int i=0;i<6;i+=1) {
    Serial.print(adrMAC[i]);
    Serial.print(":");
  }
  Serial.println();
  Serial.print("ip : ");
  ip = WiFi.localIP();
  Serial.println(ip);
  Serial.print("gw : ");
  gw = WiFi.gatewayIP();
  Serial.println(gw);
  Serial.print("mask : ");
  masque = WiFi.subnetMask();
  Serial.println(masque);
}


void setup() {
  Serial.begin(9600);
  delay(1000);
  ConnexionWifi((char*)ssid,password);
  AfficheParametres();
  delay(1000);
}

void loop() {
	delay(5000);
	
	Serial.print("connecting to ");
	Serial.println(host);
	
	/ envoyer le requete au server
	WiFiClient client;
	const int httpPort = 80;
	if (client.connect(host, httpPort)) {
		Serial.println("envoi requete http GET");
		// envoyer le requete au server
		client.print(requete);
		Serial.println("Reponse: ");
		While(client.connected() || client.available()) {
		  	if(client.available()) {
		  		// Lisez toutes les lignes de la reponse du serveur et imprimez-les sur Serial
			  	String line = client.readStringUntil('\n');
			  	Serial.println(line);
			  	}
		  	}
	  		client.stop();
	  		Serial.println("Deconnection");
	}
	else {
		Serial.println("connection echouee");
	  	client.stop();
	}
}
