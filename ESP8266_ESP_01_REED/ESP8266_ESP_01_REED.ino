#include <ESP8266WiFi.h>

const char* ssid = "TR-2";
const char* password = "1234";
const char* host = "maker.ifttt.com";
const char* iftttApiKey = "bHqghGuQv0nZhfaBKSVZ-v";//IFTTT API SECRET  KEY
const char* door_state = "closed";

const int pin = 2;//GPIO2
volatile int state = false;
volatile int flag = false;
const long interval = 3000;
unsigned long previousMillis = 0; 

void doorStatusChange() {

    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;   
    
        state = !state;
        if(state) {
            door_state = "opened";
        }
        else{
            door_state = "closed";
        }
        flag = true;
        Serial.println(state);
        Serial.println(door_state);
    }    
}

void setup() {
    Serial.begin(115200);
    pinMode(pin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(pin), doorStatusChange, CHANGE);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);    
    WiFi.begin(ssid, password);    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }  
    Serial.println();
    Serial.println("WiFi connected");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  
}

void loop() {

      if(flag){
          Serial.print("Connecting to ");
          Serial.println(host);          
          WiFiClient client;          
          if (!client.connect(host, 80)) {
            Serial.println("connection failed");
            return;
          }
    
          String url = "/trigger/front_door_opened/with/key/";
          url += iftttApiKey;
          
          Serial.print("URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + door_state + "\r\n");
          flag = false;
      }  
      delay(10);
}
