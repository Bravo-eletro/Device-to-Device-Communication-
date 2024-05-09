// ESP32 BMP280 SERVER

#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// WiFi credentials
const char* ssid = "Your SSID";
const char* password = "Your Password"; 


// Web server
WebServer server(80);

// BMP280 sensor
Adafruit_BMP280 bmp;

// OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Global variable to hold the server IP address
String serverIPAddress;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Get the server's IP address
  serverIPAddress = WiFi.localIP().toString();
  Serial.println("Server IP: " + serverIPAddress);

  // Initialize BMP280 sensor
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor!");
    while (1);
  }

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Failed to initialize OLED display!");
    while (1);
  }

  display.clearDisplay();
  display.display();

  // Web server route to serve BMP280 data
  server.on("/data", []() {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F; // hPa
    String response = "Temperature: " + String(temperature) + " C, ";
    response += "Pressure: " + String(pressure) + " hPa";
    server.send(200, "text/plain", response);
  });

  server.begin();
}

void updateDisplay() {
  display.clearDisplay();

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; // hPa

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Server IP:");
  display.setCursor(0, 10);
  display.print(serverIPAddress);  // Display the server's IP address

  display.setCursor(0, 30);
  display.print("Temperature:");
  display.setCursor(0, 40);
  display.print(temperature);
  display.print(" C");

  display.setCursor(0, 50);
  display.print("Pressure:");
  display.setCursor(60, 50);
  display.print(pressure);
  display.print(" hPa");

  display.display();
}

void loop() {
  server.handleClient(); // Handle web server requests
  updateDisplay();       // Update OLED display with sensor data and IP address

  delay(1000); // Adjust delay for display refresh rate
}
