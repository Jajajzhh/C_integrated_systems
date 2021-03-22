#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include <SPI.h>
#include <Wire.h>


// initialisation de I2C
// ==================================================================== 
// initialisation les variables de I2C
float T_I2C;
float H_I2C;
int inttemp;
int humidite_status;

//les adresses et registres statique declarer
const int Hygro_Address = 0x40;
const int Hygro_reg_temp = 0x00;
const int Hygro_reg_humidite = 0x01;
const int Hygro_reg_ctrl = 0x02;
const int Hygro_init_msb = 0x00;
const int Hygro_init_lsb = 0x00;
unsigned int readHygro(byte registre);

// le methode pour recevoir les valeurs de i2chygro
unsigned int readHygro(byte registre){
	uint8_t recu[2];
	//Indiquer le registre
	Wire.beginTransmission(Hygro_Address);
	Wire.write(registre);
	Wire.endTransmission();
	delay(13);
	//Recu le donne et le transferer
	Wire.requestFrom(Hygro_Address,2);
	if(Wire.available()==2){
		recu[0]=Wire.read();
		recu[1]=Wire.read();
		return (((unsigned int)recu[0] << 8 | (unsigned int)recu[1]));
	}
	else {
		return -1;
	}
}

// calculez le resultat de I2C final 
void get_humi_temp() {
	//On utilise la fonction readHygro pour obtenir des donnes en donnant le registre
	unsigned int data = readHygro(Hygro_reg_temp);
	//Interpretation de donnes en temperature
    T_I2C = (float) (data)/(65536)*165-40;
    inttemp=(int)T_I2C;
    //Puis on donne le registre humidite pour obtenir des donnes humidite
    data = readHygro(Hygro_reg_humidite);
    //Interpretation de donne humidite
    H_I2C = data/(65536.0f)*100.0f;
    //Ecrire la temperature et l'humidite dans Console
    Serial.println("temp: ");
    Serial.println(T_I2C);
    Serial.println("humi: ");
    Serial.println(H_I2C);
}



// initialisation de SPI
// ==================================================================== 
// initialisation les variables de I2C
uint8_t arr[5];
int i;
uint32_t pres;
uint16_t temp;
uint8_t statT;
uint8_t statP;
float P_SPI;
float T_SPI;

//les adresses et registres statique declarer
const int WHO_AM_I = 0x0F;
const int CTRL_RES_CONF = 0x10;
const int CTRL_REG1 = 0x20;
const int CTRL_REG2 = 0x21;
const int STATUS_REG = 0x27 | 0x80;		//~ RW = 1
const int VAL_REG[5] = {0xE8, 0xE9, 0xEA, 0xEB, 0xEC};	//~ RW = 1 MS = 1



//~ configuration les registres
void set_ctrl(){
	//~ configuration de la resolution
	digitalWrite(SS, LOW);
	SPI.transfer(CTRL_RES_CONF);
	SPI.transfer(0x0A);
	digitalWrite(SS, HIGH); 
	
	//~ configuration les registres de ctrl
	digitalWrite(SS, LOW);
	SPI.transfer(CTRL_REG1);
	SPI.transfer(0x80);
	digitalWrite(SS, HIGH); 
	
	digitalWrite(SS, LOW);
	SPI.transfer(CTRL_REG2);
	SPI.transfer(0x01);
	digitalWrite(SS, HIGH); 
}

//~ Lecture des 3 octets de pression
void read_pres(){
	statP = 0;
	while(statP != 2) {
		digitalWrite(SS, LOW);
		SPI.transfer(STATUS_REG);
		statP = SPI.transfer(0) & 0x02;		//~ lecture de status
		digitalWrite(SS, HIGH);
		delay(1);
	}
	
	digitalWrite(SS, LOW);
	SPI.transfer(VAL_REG[0]);
	for(i=0; i<3; i++){
		arr[i] = SPI.transfer(0);
	}
	digitalWrite(SS, HIGH);
}

//~ Lecture des 2 octects de temperature 
void read_temp(){
	statT = 0;
	while(statT != 1) {
		digitalWrite(SS, LOW);
		SPI.transfer(STATUS_REG);
		statT = SPI.transfer(0) & 0x01;		//~ lecture de status
		digitalWrite(SS, HIGH);
	}
	
	digitalWrite(SS, LOW);
	SPI.transfer(VAL_REG[3]);
	for(i=3; i<5; i++){
		arr[i] = SPI.transfer(0);
	}
	digitalWrite(SS, HIGH);
}

// calculez le resultat de SPI final 
void get_pres_temp() {
	set_ctrl();
	read_pres();
	read_temp();
	
	//~ combiner les premiers 3 octects pour obtenir la valeur de pression
	pres = ((arr[2] << 16) | (arr[1] << 8) | arr[0]); 
	//~ combiner les 2 octects derniers pour obtenir la valeur de temperature
	temp = ((arr[4] << 8) | arr[3]);
	
	//~ calcul de la pression
	P_SPI = (float) (pres) / 4096;
	//~ calcul de la temperature
	T_SPI = (float) (temp) / 480.0 - 94.035;
	
	Serial.print("Pres = ");
	Serial.print(P_SPI);
	Serial.print("\nTemp = ");
	Serial.print(T_SPI);
	Serial.println("\n");
}

// initialisation de WiFi
// ==================================================================== 
// initialisation les constantes de WiFi
const char ssid[20]     = "d206-IOT-AP";
const char password[20] = "d2062019uttlo11";
const char host[20] = "10.23.12.199";
const int httpPort = 80;

float T = (T_SPI + T_I2C) / 2;

// initialisation un WiFiServer 
WiFiServer server(httpPort);

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

// executer le processus d'envoi
// ==================================================================== 

void setup() {
	Serial.begin(9600);
	// initialisation de I2C
	Wire.begin();
	Wire.beginTransmission(Hygro_Address);
    Wire.write(Hygro_reg_ctrl);
    Wire.write(Hygro_init_msb);
    Wire.write(Hygro_init_lsb);
    Wire.endTransmission();
	
	// initialisation de SPI
	SPI.begin();
	delay(15);
	SPI.setDataMode(SPI_MODE0); 			//~ choisir Mode0
	SPI.setClockDivider(SPI_CLOCK_DIV8); 	//~ vitesse horloge
	SPI.setBitOrder(MSBFIRST);  			//~ ordre des bits
	pinMode(SS,OUTPUT); 			
	delay(50);
	
	// initialisation de WiFi
	delay(1000);
	ConnexionWifi((char*)ssid,password);
	AfficheParametres();
	server.begin(); //demarrer le server
	delay(1000);
}

void loop() {
	delay(5000);
	get_pres_temp();
	get_humi_temp();
	// le requete pour envoyer les valeurs
	String requete = "GET /xieyuxin/cgi-bin/ex3.cgi?temperature=" + String(T, 2) + "&humidite=" + String(H_I2C, 2) + "&pression=" + String(P_SPI, 2) + " HTTP/1.0\r\n\r\n";
	Serial.print("connecting to ");
	Serial.println(host);
	
	WiFiClient client = server.available();
	if (client.connect(host, httpPort)) {
		Serial.println("envoi requete http GET");
		// envoyer le requete au server
		client.print(requete);
		Serial.println("Reponse: ");
		while(client.connected() || client.available()) {
		  	if(client.available()) {
		  		// Lisez toutes les lignes de la reponse du serveur et imprimez-les sur Serial
			  	String line = client.readStringUntil('\n');
			  	Serial.println(line);
			  	}
		  	}
	  		client.stop();
	  		Serial.println("Deconnection");
	  		// verification de la requete
		    if(line == "OK")
		    	Serial.println("Transmission OK");
		    else
		    	Serial.println("Transmission Error");
	}
	else {
		Serial.println("connection echouee");
	  	client.stop();
	}
}

