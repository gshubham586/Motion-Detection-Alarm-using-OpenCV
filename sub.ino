#include <ESP8266WiFi.h>
#include <PubSubClient.h>



const char* ssid = "Shubham";                    // Enter wi-fi SSID
const char* password = "11111111";               //Enter wi-fi password

const char* mqtt_server = "192.168.43.121";     // Enter server IP or localhost IP address (If you are using a local mqtt broker) 
//const int mqttPort = 1883;                       // Port for MQTT protocol

WiFiClient espClient2;
PubSubClient client(espClient2);


int i=1;
String datasensor;      



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  
  Serial.begin(115200);                                           //Now 8-9 data per second,   Change Baudrate to receive data at different speed
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);                     
 
  Serial.print("Connecting to ");
  Serial.println(ssid);                                   
  WiFi.begin(ssid, password);                                     // Connecting to wi-fi
  while (WiFi.status() != WL_CONNECTED) {                 
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
  

 while(!Serial) {}

   
  
}




void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}


void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
  
