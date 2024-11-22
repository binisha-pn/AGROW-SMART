# AGROW-SMART
An innovative hardware model that aids precision agriculture through:
- Soil nutrition testing (NPK analysis)
- Fertilization recommendations
  
## Features
- **Soil Testing:** Uses sensors and RS485 to measure NPK values.
- **Fertilization Recommendations:** Provides customized strategies based on detected soil deficiencies.

  ## Technologies Used
- **Arduino:** Controls the sensors and processes data.
- **RS485 Communication Protocol:** Interfaces between sensors and Arduino.
- **Sensors:** Measures soil nutrient levels (NPK).

## Circuit Diagram
![Circuit Diagram]()

## How It Works
1. **Sensors**: The RS485 sensor collects soil data (NPK levels).
2. **Microcontroller**: Arduino processes the data and analyzes deficiencies.
3. **Output**: Displays results and recommendations on an LCD screen.
4. **Action**: Farmers can adjust soil treatment based on the provided recommendations.

  ### Prerequisites
To use this project, ensure you have the following installed:

- **Arduino IDE** (download from [here](https://www.arduino.cc/en/software)).
- Required Arduino libraries:
  - [`SoftwareSerial`](https://www.arduino.cc/en/Reference/softwareSerial) (built-in with Arduino IDE).
  - [`Wire`](https://www.arduino.cc/en/reference/wire) (built-in with Arduino IDE).
  - [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) (install via Arduino Library Manager or from the linked GitHub repository).

#### Installing Libraries
1. Open Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries**.
3. Search for `LiquidCrystal_I2C` and install it.

## Steps to Replicate
1. Assemble the components as per the circuit diagram.
2. Connect the NPK sensor to the Arduino as per the specifications.
3. Upload the `code.ino` file to the Arduino board.
4. Power the system and monitor the results displayed on the LCD.

   ## Components Used
1. Arduino Uno
2. NPK Sensor
3. RS485
4. Power Supply (5V)
5. LCD Display
6. Additional components (e.g., button, connectors)

## Future Enhancements
- Add IoT capability for remote monitoring.
- Extend recommendations for other soil properties like pH and moisture levels.
- Integrate a mobile or web application for enhanced user experience.
