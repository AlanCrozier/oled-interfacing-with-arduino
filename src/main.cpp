/**
 * @file main.cpp
 * @author Your Name
 * @date 2026-02-11
 * @brief Arduino sketch for OLED display with DHT22 sensor integration.
 *
 * This project demonstrates integration of an SSD1306 OLED display (128x64)
 * with an I2C interface and a DHT22 temperature/humidity sensor. The system
 * reads sensor data and displays initialization messages on the OLED screen
 * while continuously monitoring temperature and humidity values via the
 * serial interface.
 *
 * @details
 * - **OLED Display**: Adafruit SSD1306 (128x64) connected via I2C
 * - **Temperature/Humidity Sensor**: DHT22 (AM2302)
 * - **Microcontroller**: Arduino UNO R4
 * - **Protocol**: I2C (Wire) for OLED, DHT protocol for sensor
 *
 * @see https://github.com/adafruit/Adafruit_SSD1306
 * @see https://github.com/adafruit/DHT-sensor-library
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

/**
 * @defgroup DisplayConfig OLED Display Configuration
 * @brief Configuration parameters for the SSD1306 OLED display.
 * @{
 */
/// Display width in pixels
#define SCREEN_WIDTH 128
/// Display height in pixels
#define SCREEN_HEIGHT 64
/// I2C address of the OLED display
#define OLED_ADDR 0x3C
/** @} */

/**
 * @defgroup SensorConfig DHT22 Sensor Configuration
 * @brief Configuration parameters for the DHT22 temperature/humidity sensor.
 * @{
 */
/// Digital pin connected to the DHT sensor data line
#define DHTPIN 2
/// DHT sensor type identifier (DHT22 = AM2302)
#define DHTTYPE DHT22
/** @} */

/// @brief OLED display object for 128x64 SSD1306 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);
/// @brief DHT22 sensor object for temperature and humidity readings
DHT dht(DHTPIN, DHTTYPE);
/// @brief Counter for tracking consecutive sensor errors
uint8_t errorCount = 0;
/// @brief Maximum consecutive errors before display warning
#define MAX_ERRORS 3
/**
 * @brief Initializes hardware and peripherals for the Arduino application.
 *
 * This function performs the following initialization tasks:
 * - Initializes serial communication at 9600 baud for debugging output
 * - Begins DHT22 sensor operation
 * - Initializes the SSD1306 OLED display via I2C
 * - Configures display parameters (size, color, cursor position)
 * - Displays initial welcome messages on the OLED screen
 *
 * @note If OLED initialization fails, the application enters an infinite loop
 *       as continued operation without display feedback is not viable.
 * @note Serial communication must be established for debugging temperature
 *       and humidity readings.
 * @warning Ensure proper I2C pull-up resistors (4.7kΩ) are connected on
 *          the SDA and SCL lines for reliable I2C communication.
 *
 * @return void
 *
 * @see DHT::begin()
 * @see Adafruit_SSD1306::begin()
 */
void setup() {
    // Initialize serial communication for debugging output
    Serial.begin(9600);

    // Initialize DHT22 sensor
    dht.begin();

    // Initialize OLED display with I2C address and error handling
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED not found");
        while (true); // Loop forever if initialization fails
    }

    // Configure display appearance
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    // Display startup messages
    display.println("Arduino UNO R4");
    display.println("OLED with I2C");
    display.println("Hello I2C");
    display.display();

    // Log initialization completion to serial monitor
    Serial.println("DHT22 init");
}

/**
 * @brief Main application loop - continuously reads and displays sensor data.
 *
 * This function is called repeatedly by the Arduino runtime and performs
 * the following operations:
 * - Reads temperature and humidity values from the DHT22 sensor
 * - Validates sensor readings (checks for NaN values indicating sensor errors)
 * - Implements retry logic for transient sensor failures
 * - Displays readings continuously on the OLED screen and serial monitor
 * - Provides diagnostic information for troubleshooting sensor issues
 *
 * @details
 * **Sensor Reading Operation:**
 * - DHT22 sensors require at least 2 seconds between readings for accurate results
 * - The sensor may occasionally return invalid (NaN) values due to timing issues
 * - Retry mechanism handles transient communication failures
 *
 * **Display Output:**
 * - OLED displays real-time temperature and humidity with continuous updates
 * - Display is cleared and refreshed on each iteration for clean output
 * - Serial monitor outputs formatted readings for debugging
 * - Error counter tracks consecutive failures
 *
 * **Error Handling:**
 * - Transient errors (< MAX_ERRORS) trigger retry without display warning
 * - Persistent errors (>= MAX_ERRORS) display troubleshooting guidance
 * - Provides pin, wiring, and power supply diagnostics
 * - Auto-recovery when sensor returns valid readings
 *
 * @return void
 *
 * @note The 2000ms delay between readings prevents sensor saturation and
 *       provides time for the DHT sensor's internal capacitors to stabilize.
 * @note OLED display is continuously updated with sensor values for real-time feedback.
 * @note Check DHT22 wiring: GND, VCC (3.3-5V), and DATA pin connections
 *
 * @see DHT::readTemperature()
 * @see DHT::readHumidity()
 * @see isnan()
 */
void loop() {
    // Read temperature in Celsius from DHT22 sensor
    float temp = dht.readTemperature();

    // Read humidity percentage from DHT22 sensor
    float hum = dht.readHumidity();

    // Validate sensor readings - NaN indicates sensor communication error
    if (isnan(temp) || isnan(hum)) {
        errorCount++;
        Serial.print("Sensor read failed (Error #");
        Serial.print(errorCount);
        Serial.println(")");

        // Display based on error persistence
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        if (errorCount < MAX_ERRORS) {
            // Transient error - show brief message and retry
            display.println("Sensor Reading...");
            display.println("Please wait...");
            display.println();
            display.print("Retry: ");
            display.print(errorCount);
            display.print("/");
            display.println(MAX_ERRORS);
        } else {
            // Persistent error - show diagnostics
            display.println("SENSOR ERROR!");
            display.println();
            display.println("Check:");
            display.println("- GND connection");
            display.println("- VCC (3.3-5V)");
            display.println("- DATA pin (Pin 2)");
            display.println("- Pull-up resistor");
        }

        display.display();

        delay(2000);
        return;
    }

    // Reset error counter on successful read
    if (errorCount > 0) {
        errorCount = 0;
        Serial.println("Sensor recovered!");
    }

    // Clear OLED display for fresh data
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    // Display temperature reading on OLED continuously
    display.print("Temp: ");
    display.print(temp);
    display.println(" C");

    // Display humidity reading on OLED continuously
    display.print("Humidity: ");
    display.print(hum);
    display.println(" %");

    // Display reading status
    display.println();
    display.println("Status: OK");

    // Update OLED display with new sensor values
    display.display();

    // Output temperature reading to serial monitor
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C ");

    // Output humidity reading to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");

    // Allow DHT22 sensor sufficient time to stabilize between readings
    delay(2000);
}