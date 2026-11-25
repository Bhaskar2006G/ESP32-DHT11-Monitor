#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

#define DHTPIN 4
#define DHTTYPE DHT11


const char* ssid = "YOUR_WIFI_NAME";
const char* password = "Your_WIFI_Password";

unsigned long channelID = YOUR_CHANNEL_IDS;
const char* apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  dht.begin();

 
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);

  Serial.println("ThingSpeak Started!");
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  
  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor read failed!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.reconnect();
    delay(2000);
    return;
  }

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent successfully!");
  }
  else {
    Serial.print("ThingSpeak error: ");
    Serial.println(response);
  }

  delay(15000);
}