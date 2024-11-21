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
