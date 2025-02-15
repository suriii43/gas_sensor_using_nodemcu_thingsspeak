#include <ESP8266WiFi.h>

String apiKey = "TY5D2B5AT9PK979B"; // Enter your Write API key from ThingSpeak

const char *ssid = "qwer123";  // Replace with your WiFi SSID
const char *pass = "qwer1234"; // Replace with your WiFi password
const char *server = "api.thingspeak.com";

#define MQ135_PIN A0  // Pin where the MQ-135 sensor is connected (usually analog pin A0)

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Read the analog value from MQ-135 sensor
  int mq135Value = analogRead(MQ135_PIN);

  // Optionally, map the analog value to a more meaningful range (depending on sensor calibration)
  // For now, we're sending the raw analog value.
  
  Serial.print("MQ-135 Sensor Value: ");
  Serial.println(mq135Value);

  // If we can connect to the ThingSpeak server, send the data
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1="; // Field 1 will hold the MQ-135 value
    postStr += String(mq135Value);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    // Print the sent data to Serial Monitor for debugging
    Serial.print("Sending data to ThingSpeak: ");
    Serial.println(postStr);

    client.stop(); // Close the connection
  }

  // Wait for 15 seconds before sending the next data (ThingSpeak rate limit)
  Serial.println("Waiting for the next update...");
  delay(500);  // ThingSpeak allows updates every 15 seconds
}
