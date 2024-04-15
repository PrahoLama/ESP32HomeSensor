#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // No reset pin

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT   dht(25, DHT11);
// RGB LED pins
#define BLUE 17
#define GREEN 5
#define RED 18

// Menu options
const char *menuOptions[] = {"Temperatura", "Umiditate", "Intensitatea luminii"};
const int numOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);

// Button pins
const int buttonPin = 2;        // The common button pin
const int optionButtonPin = 4;  // Pin for the specific option button

int selectedOption = 0;
bool showInfo = false; // Flag to control whether to show information permanently

void setup() {
   dht.begin();
  pinMode(buttonPin, INPUT_PULLUP);       // Set common button pin as input with internal pull-up resistor
  pinMode(optionButtonPin, INPUT_PULLUP); // Set specific option button pin as input with internal pull-up resistor

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // Initially turn on the red LED
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  Serial.begin(9600);

  if (!display.begin()) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
  displayMenu();
}

void loop() {

 
  // Check if the common button is pressed
  if (digitalRead(buttonPin) == LOW && !showInfo) {
    // Button is pressed and not showing info, move to the next menu option
    selectedOption = (selectedOption + 1) % numOptions;
    displayMenu();
    delay(200); // Debouncing delay
  }

  // Check if the specific option button is pressed
  if (digitalRead(optionButtonPin) == LOW) {
    if (!showInfo) {
      // Button is pressed and not showing info, set showInfo flag to true
      showInfo = true;
      displayInfo();
    } else {
      // Button is pressed and showing info
      showInfo = false;
      if (selectedOption == 0)
        selectedOption = 0; // Reset selectedOption to 0 when returning to the main menu
      else if (selectedOption == 1)
        selectedOption = 1;
      else if (selectedOption == 2)
        selectedOption = 2;
      displayMenu();
    }
    delay(200); // Debouncing delay
  }

  float temperature = dht.readTemperature();

// Control RGB LED based on temperature levels
if (temperature < 20) {
  // Blue color for temperatures less than 17.5°C
  Serial.println("Temperature is less than 17.5°C");
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
} else if (temperature >= 20 && temperature < 35) {
  // Green color for temperatures between 17.5°C and 30°C
  Serial.println("Temperature is between 17.5°C and 30°C");
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
} else if (temperature >= 35) {
  // Red color for temperatures greater than or equal to 30°C
  Serial.println("Temperature is greater than or equal to 30°C");
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

}

 
}

void displayMenu() {
  display.clearDisplay();

  // Set text size and color
  display.setTextSize(1.5);
  display.setTextColor(SH110X_WHITE);

  // Display menu title
  display.setCursor(0, 0);
  display.println("Meniu:");

  // Display menu options
  for (int i = 0; i < numOptions; i++) {
    if (i == selectedOption) {
      // Draw a white box around the selected option
      display.fillRect(0, 10 + i * 10 - 1, SCREEN_WIDTH, 9, SH110X_WHITE);
    }
    display.setCursor((SCREEN_WIDTH - strlen(menuOptions[i]) * 6) / 2, 10 + i * 10); // Center the text horizontally
    if (i == selectedOption) {
      // Highlight the selected option
      display.setTextColor(SH110X_BLACK);
    } else {
      display.setTextColor(SH110X_WHITE);
    }
    display.println(menuOptions[i]);
  }

  // Display the menu
  display.display();
}

void displayInfo() {
  while (showInfo) {
    // Check if the common button is pressed to go back to the main menu
    if (digitalRead(buttonPin) == LOW) {
      showInfo = false; // Set showInfo to false to exit the loop
      displayMenu(); // Display the main menu
      return; // Exit the function
    }
  float temperature = ReadTemperature();
  display.clearDisplay();
  display.setTextSize(1.2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  if (selectedOption == 0) {
    display.clearDisplay();
    display.setTextSize(3); // Set text size to 3 for bigger text
    display.setTextColor(SH110X_WHITE);

    // Calculate the position to center the text horizontally and vertically
    int16_t textWidth = display.getCursorX(); // Get the width of the text
    int16_t textHeight = display.getCursorY(); // Get the height of the text
    int16_t xPos = (SCREEN_WIDTH - textWidth ) / 10; // Calculate the X position to center horizontally
    int16_t yPos = (SCREEN_HEIGHT - textHeight) / 3; // Calculate the Y position to center vertically

    // Set the cursor position
    display.setCursor(xPos, yPos);

    // Print the temperature with one decimal place
    display.print(temperature, 1);
    display.print(" C "); // Print the "C" symbol next to the temperature

    display.setTextSize(1); // Set text size back to 1
  } else if (selectedOption == 1) {
    float humidity = ReadHumidity();
    display.clearDisplay();
    display.setTextSize(3); // Set text size to 3 for bigger text
    display.setTextColor(SH110X_WHITE);

    // Calculate the position to center the text horizontally and vertically
    int16_t textWidth = display.getCursorX(); // Get the width of the text
    int16_t textHeight = display.getCursorY(); // Get the height of the text
    int16_t xPos = (SCREEN_WIDTH - textWidth ) / 10; // Calculate the X position to center horizontally
    int16_t yPos = (SCREEN_HEIGHT - textHeight) / 3; // Calculate the Y position to center vertically
 
    // Set the cursor position
    display.setCursor(xPos, yPos);

    // Print the humidity with one decimal place
    display.print(humidity, 1);
    display.print(" % "); // Print the "%" symbol next to the humidity

    display.setTextSize(1); // Set text size back to 1
    
  } else if (selectedOption == 2) {
    int lightIntensity = analogRead(34);
    int brightness = map(lightIntensity, 0, 1000, 0, 100);
   display.clearDisplay();
    display.setTextSize(3); // Set text size to 3 for bigger text
    display.setTextColor(SH110X_WHITE);

    // Calculate the position to center the text horizontally and vertically
    int16_t textWidth = display.getCursorX(); // Get the width of the text
    int16_t textHeight = display.getCursorY(); // Get the height of the text
    int16_t xPos = (SCREEN_WIDTH - textWidth ) / 3; // Calculate the X position to center horizontally
    int16_t yPos = (SCREEN_HEIGHT - textHeight) / 3; // Calculate the Y position to center vertically

    // Set the cursor position
    display.setCursor(xPos, yPos);
    display.println(brightness);
  
    display.setTextSize(1); // Set text size back to 1
  }
  display.display();
}
}

float ReadTemperature() {

  float temperature = dht.readTemperature();

  return temperature;
}
float ReadHumidity()
{
   float hum = dht.readHumidity();
   return hum;
}

