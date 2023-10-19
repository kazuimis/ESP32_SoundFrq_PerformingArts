#include "arduinoFFT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SAMPLES 128
#define SAMPLING_FREQUENCY 2048

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

#define SCREEN_WIDTH 128  // Adjust based on your OLED screen resolution
#define SCREEN_HEIGHT 64  // Adjust based on your OLED screen resolution

// Define the correct I2C address for your OLED display
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C  // Replace with your OLED's I2C address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() 
{
    Serial.begin(115200);
    samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS))  // Remove OLED_RESET
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
}

void loop() 
{  
    for (int i = 0; i < SAMPLES; i++)
    {
        microSeconds = micros();
        vReal[i] = analogRead(A0); // Connect your sound sensor to analog pin A0 on the ESP32
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod))
        {
            // Wait for the next sampling period
        }
    }

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    
    // Clear the display and display the frequency
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Frequency: ");
    display.print(peak, 2); // Display the frequency value with 2 decimal places
    display.print(" Hz");
    display.display();

    // Print the frequency to the serial monitor
    Serial.print("Frequency: ");
    Serial.print(peak, 2); // Display the frequency value with 2 decimal places
    Serial.println(" Hz");

    // delay(500); // Adjust the delay for display update rate
}
