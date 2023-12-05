#include "secrets.h"
#include "DHT.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
/*#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
*/


// Define pin connections
#define DHTPIN 4  // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"


DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
/*
    // Initialize OTA
  ArduinoOTA.begin();

    // Initialize Multicast DNS
  MDNS.begin("esp32");  // Set a unique name for your ESP32

    // Check for any errors during initialization
  if (!MDNS.begin("esp32")) {
  Serial.println("Error setting up MDNS responder!");
  while (1) {
    delay(1000);
  }
}
*/



  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier (Network port 5353)
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}
// Move 
void publishMessage() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  StaticJsonDocument<200> doc;
  doc["Humidity"] = h;
  doc["Temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // Print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println(jsonBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char *message = doc["message"];

  Serial.println(message);

}

// Setup DHT22 sensor
void setupDHT() {
  //pinMode(fan, OUTPUT);
  Serial.begin(9600); 
  dht.begin();
}


void setup() {
  setupDHT();
  connectAWS();
}

void loop() {
 // ArduinoOTA.handle();
 // loopDHT();
    // Wait a few seconds between measurements.
  delay(2000);
  publishMessage();
  client.loop();
}
