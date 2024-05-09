// This is a Client 2. Code Run on ESP8266
// written on : 29/ 04 /2024

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi credentials
const char* ssid = "Swapnonil"; // Cryptappz_Lab // Swapnonil // TP-LINK_612A
const char* password = "$ubho&$hreya"; // cryp98745 // $ubho&$hreya  // 19839448

// Server's IP address or hostname
const char* serverAddress = "192.168.0.139"; // Example: "192.168.1.100"


//Relay and LED pins
#define RELAY_PIN 13  // Relay connected to D7 : GPIO 13
#define LED_PIN 12    // LED connected to D6 : GPIO 12


//--------------
//
//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//
//// WiFi credentials
//const char* ssid = "Your_SSID";
//const char* password = "Your_PASSWORD";
//
//// Server's IP address or hostname
//const char* serverAddress = "Server_IP_Address"; // Example: "192.168.1.100"
//
//// Relay and LED pins
//#define RELAY_PIN D8  // Relay connected to D8
//#define LED_PIN D7    // LED connected to D7

// Create a WiFiClient instance
WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);

  // Set up relay and LED pins
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize relay and LED states
  digitalWrite(RELAY_PIN, LOW); // Relay off by default
  digitalWrite(LED_PIN, LOW);   // LED off by default

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Use WiFiClient with HTTPClient::begin
    http.begin(wifiClient, "http://" + String(serverAddress) + "/data");
    int httpCode = http.GET();

    if (httpCode > 0) { // If the HTTP request is successful
      String payload = http.getString();
      Serial.println("Server response: " + payload);

      // Parse temperature from the server response
      int tempIndex = payload.indexOf("Temperature:");
      if (tempIndex != -1) {
        String temperatureString = payload.substring(tempIndex + 12, payload.indexOf(" C", tempIndex));
        float temperature = temperatureString.toFloat();

        // Control the relay and LED based on temperature
        // temp is greater than 30
        if (temperature > 36.0) {
          digitalWrite(RELAY_PIN, HIGH); // Turn on the relay
          digitalWrite(LED_PIN, LOW);   // Turn off the LED
        } else {
          digitalWrite(RELAY_PIN, LOW); // Turn off the relay
          digitalWrite(LED_PIN, HIGH);  // Turn on the LED
        }
      }
    } else {
      Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); // Close HTTP connection

  } else {
    Serial.println("Disconnected from WiFi");
  }

  delay(5000); // Delay for 5 seconds before checking again
}
