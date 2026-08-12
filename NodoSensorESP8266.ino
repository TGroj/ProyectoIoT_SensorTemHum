//librerias 
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
//sensores 
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht = DHT(DHTPIN, DHTTYPE);

//Credenciales WiFi
char ssid[] = "SITHOME";
char pass[] = "sitnet2019.";
//Direccion servidor MQTT
const char* mqtt_server = "192.168.1.199";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

float temp, hum; //variables de medicion

void setup_wifi(){
  delay(100);
  //inicar wifi
  Serial.println();
  Serial.print("conectando a: ");
  Serial.println(ssid);

  //WiFi.init(AP_STA_MODE);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println();
  Serial.println("Wifi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length){}

void reconnect() {
  //ciclo hasta conectarse
  while(!client.connected()){
    Serial.print("intentando conexion a MQTT...");
    //crear ID de cliente aleatorio
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    //intentando conexion
    if (client.connect(clientId.c_str())){
      Serial.println("conectado");
      //una vez conectado, publica un anuncio
      client.publish("outTopic", "hola mundo");
      // y resuscribe
      client.subscribe("inTopic");
    }else{
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println("intenta de nuevo en 5 segundos");
      //espera 5 segundos
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println(F("test sensorDHT"));
  dht.begin(); //inicializar sensor

  setup_wifi(); //conectar WiFi
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  StaticJsonDocument<32> doc; //creacion JSON
  char output[55];

  long now = millis();
  if(now - lastMsg > 5000){
    lastMsg = now;
   temp = dht.readTemperature();
   hum = dht.readHumidity();
    
    doc["t"] = temp;
    doc["h"] = hum;

    Serial.println("Read");

    serializeJson(doc, output);
    Serial.println(output);
    client.publish("/home/sensor", output); 
    Serial.println("Sent");//publica JSON
  }
}
