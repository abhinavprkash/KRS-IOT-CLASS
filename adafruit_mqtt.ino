#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4

#define WLAN_SSID       "AbhinavmiA3"             // Your SSID
#define WLAN_PASS       "aaaaaaaa"        // Your password

/********* Adafruit.io Setup ***********/

#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "abhinavprkash"            // Username
#define AIO_KEY         "aio_mTKi86QYIXlFNRbUTITVoCbNNFnl"   // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Switch1"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Switch2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Switch3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Switch4");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light4);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      Serial.print(F("Got1: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      //(Light1_State == HIGH)? Light1_State = 0 : Light1_State =1;
      digitalWrite(Relay1, Light1_State);
      
    }
    if (subscription == &Light2) {
      Serial.print(F("Got2: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      //(Light2_State == HIGH)? Light2_State = 0 : Light2_State =1;
      digitalWrite(Relay2, Light2_State);
    }
    if (subscription == &Light3) {
      Serial.print(F("Got3: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      //(Light3_State == HIGH)? Light3_State = 0 : Light3_State =1;
      digitalWrite(Relay3, Light3_State);
    }
    if (subscription == &Light4) {
      Serial.print(F("Got4: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      //(Light4_State == HIGH)? Light4_State = 0 : Light4_State =1;
      digitalWrite(Relay4, Light4_State);
      
    }
  }

  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
