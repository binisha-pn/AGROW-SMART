#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RE 8
#define DE 7
#define BUTTON_PIN_UP     4 // Pin for scrolling up
#define BUTTON_PIN_DOWN   5 // Pin for scrolling down
#define BUTTON_PIN_SELECT 6 // Pin for selecting crop

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];
SoftwareSerial mod(2, 3);
byte val1, val2, val3;
float measuredNitrogen, measuredPhosphorous, measuredPotassium;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Arrays to store crop names and their respective NPK requirements
String cropNames[] = {"Banana", "Coconut", "Rubber", "Pepper", "Tea", "Coffee", "Pineapple",
                      "Ginger", "Cardamom", "Turmeric", "Cashew", "Mango", "Jackfruit", "Tapioca", "Rice", "Sugarcane",
                      "Papaya", "Pineapple", "Papaya", "Turmeric", "Banana", "Pepper", "Pineapple", "Papaya", "Rubber"};

float nitrogenRequirements[] = {6, 3, 5, 10, 4, 4, 5, 3, 2, 3, 1, 2, 2, 2, 3, 4, 8, 5, 8, 3, 6, 10, 5, 8, 5};
float phosphorousRequirements[] = {1.6, 1, 1, 4, 1, 1, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 6, 3, 6, 1, 1.6, 4, 3, 6, 1};
float potassiumRequirements[] = {11.7, 6, 6, 10, 400, 3, 10, 3, 3, 2, 1, 1, 3, 3, 2, 3, 6, 10, 6, 2, 11.7, 10, 10, 6, 6};
int sel_crop[5];  //an array capable of holding 20 entries numbered 0 to 19
byte sel_index = 0;
int selectedCropIndex;
int numCrops = sizeof(sel_crop) / sizeof(sel_crop[0]);

void setup() {
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(BUTTON_PIN_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SELECT, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NPK Analysing");
  delay(2000);
  val1 = nitrogen();
  delay(500); // Delay added to stabilize measurement
  val2 = phosphorous();
  delay(500); // Delay added to stabilize measurement
  val3 = potassium();
  delay(500); // Delay added to stabilize measurement
  lcd.clear();
}

void loop() {
  val1 = nitrogen();
  delay(500); // Delay added to stabilize measurement
  val2 = phosphorous();
  delay(500); // Delay added to stabilize measurement
  val3 = potassium();
  delay(500); // Delay added to stabilize measurement
  display();
  // Measured NPK values
  measuredNitrogen    = val1;
  measuredPhosphorous = val2;
  measuredPotassium   = val3;
  crop_selector();
}

void crop_selector() {
  // Check if measured NPK values are greater than required for each crop
  int suitableCrops = 0; // Counter for suitable crops
  for (int i = 0; i < sizeof(cropNames) / sizeof(cropNames[0]); i++) {
    if (measuredNitrogen > nitrogenRequirements[i] && measuredPhosphorous > phosphorousRequirements[i]
        && measuredPotassium > potassiumRequirements[i]) {
      Serial.print(cropNames[i]);
      Serial.println(" is suitable.");
      suitableCrops++;

      // Print the crop number and name on a new line on the LCD
      lcd.setCursor(0, 1); // Set cursor to the beginning of the second line
      lcd.print(suitableCrops); // Print the crop number
      lcd.print(". "); // Print a dot for separation
      lcd.print(cropNames[i]); // Print the crop name
      delay(1000); // Delay for better readability
      sel_crop[sel_index] = i;
      sel_index++;
      lcd.clear();
      delay(50);
    }

    // Break out of the loop if we've found five suitable crops
    if (suitableCrops >= 5) {
      break;
    }
  }

  // If less than 5 suitable crops are found, indicate need for further analysis
  if (suitableCrops < 5) {
    Serial.println("Consider further analysis for more suitable crops.");
  }

  // Main loop for scrolling through crop list and selecting a crop
  while (true) {
    // Display current crop
    lcd.setCursor(0, 1);
    lcd.print(cropNames[sel_crop[sel_index]]);
    Serial.println(cropNames[sel_crop[sel_index]]);
// Scroll up if the "up" button is pressed
    if (digitalRead(BUTTON_PIN_UP) == LOW) {
      sel_index = (sel_index - 1 + numCrops) % numCrops;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Crop List:");
      delay(250); // Debounce delay
    }

    // Scroll down if the "down" button is pressed
    if (digitalRead(BUTTON_PIN_DOWN) == LOW) {
      sel_index = (sel_index + 1) % numCrops;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Crop List:");
      delay(250); // Debounce delay
    }

    // Select the current crop if the "select" button is pressed
    if (digitalRead(BUTTON_PIN_SELECT) == LOW) {
      selectedCropIndex = sel_index;
      displayRequiredNPKValues(); // Display required NPK values for the selected crop
      break;
    }

    delay(100); // Delay for responsiveness
  }
  // Display the selected crop
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Selected Crop:");
  lcd.setCursor(0, 1);
  lcd.print(cropNames[selectedCropIndex]);
  // Wait for a while before going back to the main loop
  delay(5000);
}

void displayRequiredNPKValues() {
  // Display required NPK values for the selected crop on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Required NPK for:");
  lcd.setCursor(0, 1);
  lcd.print(cropNames[selectedCropIndex]);
  delay(2000); // Delay for better readability

  lcd.setCursor(0, 3);
  lcd.print("N: ");
  lcd.print(nitrogenRequirements[selectedCropIndex]);
  lcd.print(" mg/kg");
  delay(2000); // Delay for better readability

  lcd.setCursor(0, 3);
  lcd.print("P: ");
  lcd.print(phosphorousRequirements[selectedCropIndex]);
  lcd.print(" mg/kg");
  delay(2000); // Delay for better readability

  lcd.setCursor(0, 4);
  lcd.print("K: ");
  lcd.print(potassiumRequirements[selectedCropIndex]);
  lcd.print(" mg/kg");
  delay(2000); // Delay for better readability

  delay(3000); // Display for 3 seconds before returning to the main loop
}




byte nitrogen() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(nitro, sizeof(nitro)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte phosphorous() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(phos, sizeof(phos)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte potassium() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(pota, sizeof(pota)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

void display() {
  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nitrogen: ");
  lcd.print(val1);
  lcd.println(" mg/kg");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Phosphorous: ");
  lcd.print(val2);
  lcd.println(" mg/kg");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Potassium: ");
  lcd.print(val3);
  lcd.println(" mg/kg");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Crop List:");
}
