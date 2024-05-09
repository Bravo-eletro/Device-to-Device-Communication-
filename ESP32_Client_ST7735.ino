//This is the tested code for ESP32 WEBSERVER CLIENT. AND ST7753 tft display

// Client 1

#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// WiFi credentials
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD"; 

// Server's IP address or hostname
const char* serverAddress = "Put here the IP address of Server esp32"; // Example: "192.168.1.100"

// ST7735 tft RGB Display setup 
#define TFT_CS     5   // Chip select pin
#define TFT_RST    4   // Reset pin
#define TFT_DC     2   // Data/Command pin

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Define the scrren of text color
#define ST7735_BLACK    0x0000
#define ST7735_BLUE     0x001F
#define ST7735_RED      0xF800
#define ST7735_GREEN    0x07E0
#define ST7735_CYAN     0x07FF
#define ST7735_MAGENTA  0xF81F
#define ST7735_YELLOW   0xFFE0
#define ST7735_WHITE    0xFFFF

void setup() {
  Serial.begin(115200);

  // Initialize the ST7735 display
  tft.initR(INITR_BLACKTAB); // Initialization for a specific ST7735 variant
  tft.setRotation(0); // Rotate screen (0, 1, 2, 3 for different orientations)
  tft.fillScreen(ST7735_BLACK); // Clear the screen
  //  tft.setTextColor(ST7735_WHITE); // Set text color
  //  tft.setTextSize(1); // Set text size

  // WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    tft.fillScreen(ST7735_BLACK); // Clear the screen

    tft.setTextSize(1); // Set text size

    tft.setCursor(10, 10); // Position cursor at (x, y)
    tft.print("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  tft.fillScreen(ST7735_BLACK); // Clear the screen
  tft.setTextSize(1); // Set text size

  tft.setCursor(10, 10); // Position cursor at (x, y)
  tft.print("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Make the request
    http.begin("http://" + String(serverAddress) + "/data");
    int httpCode = http.GET();

    tft.fillScreen(ST7735_BLACK); // Clear the screen
    tft.setCursor(10, 10); // Position cursor at (x, y)

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload); // Print server response

      // Parse temperature and pressure from the payload
      int tempIndex = payload.indexOf("Temperature:");
      int presIndex = payload.indexOf("Pressure:");

      if (tempIndex != -1) {
        String temperature = payload.substring(tempIndex + 12, payload.indexOf(" C", tempIndex));

        tft.setTextColor(ST7735_BLUE); // Change text color to green
        tft.setTextSize(1); // Set text size

        tft.print("Temperature");
        tft.setCursor(10, 30); // Adjust y position for the value
        tft.setTextSize(2); // Set text size

        tft.print(temperature + " C");
      }

      if (presIndex != -1) {
        tft.setCursor(10, 50); // Adjust y position for pressure label

        tft.setTextColor(ST7735_GREEN); // Change text color to green
        tft.setTextSize(1); // Set text size

        tft.print("Pressure");
        tft.setCursor(10, 70); // Adjust y position for the value

        tft.setTextSize(1); // Set text size

        String pressure = payload.substring(presIndex + 9, payload.indexOf(" hPa", presIndex));
        tft.print(pressure + " hPa");
      }

    } else {
      Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
      tft.print("HTTP GET failed:\n");
      tft.print(http.errorToString(httpCode).c_str()); // Display error message
    }

    http.end(); // Close connection

  } else {
    Serial.println("Disconnected from WiFi");
    tft.fillScreen(ST7735_BLACK); // Clear the screen
    tft.setTextSize(1); // Set text size

    tft.setCursor(10, 10); // Position cursor at (x, y)
    tft.print("Disconnected from WiFi");
  }

  delay(10000); // Delay for 10 seconds before the next request
}
