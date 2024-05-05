#include <OneWire.h>
#include <Wire.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIN_ECHO 12
#define PIN_TRIG 11
#define PIN_LED 3
#define PIN_TEMP 2

float distance = 0;

OneWire oneWire(PIN_TEMP);           // Inicializace OneWire sběrnice na daném pinu
DallasTemperature sensors(&oneWire); // Inicializace čidla DallasTemperature

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // initialize temperature sensor
    sensors.begin();

    // initialize ultrasonic
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    // initialize LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    display.clearDisplay();
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0); // Start at top-left corner
    display.cp437(true);     // Use full 256 char 'Code Page 437' font

    display.println(F("Ultrasonic v2.1"));
    display.println(F("\n"));
    display.println(F("If you start me up"));
    display.println(F("I'll never stop..."));

    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
}

void loop()
{
    // Start a new measurement:
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    // Use 343 metres per second as speed of sound
    int duration = pulseIn(PIN_ECHO, HIGH);
    distance = duration * 0.034 / 2;

    // Turn on the LED and display the distance on the LCD
    if (distance > 0 && distance < 30)
        digitalWrite(PIN_LED, HIGH);
    else
    {
        // Turn off the LED
        digitalWrite(PIN_LED, LOW);
    }

    if (distance > 0 && distance < 20)
    {
        // Start a new measurement for temperature
        sensors.requestTemperatures();                // Požádání čidla o měření teploty
        int temperature = sensors.getTempCByIndex(0); // Přečtení teploty z čidla
        // temperature = round(temperature * 10) / 10.0; // Zaokrouhlení na jedno desetinné místo

        display.clearDisplay();
        display.setTextSize(1); // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0); // Start at top-left corner
        display.cp437(true);     // Use full 256 char 'Code Page 437' font

        display.println(F("Ultrasonic v2.1"));
        display.println(F("\n"));
        display.println("Distance:    " + String(distance) + " cm");
        display.println("Remaining:   " + String(getDays(distance)) + " day(s)");
        display.println("\n");
        display.println("Temperature: " + String(temperature) + " C");

        display.display();
        delay(2000); // Pause for 2 seconds
    }

    else
    {
        display.clearDisplay();
        display.display();
    }
}

int getDays(int distance)
{
    // Calculate the number of days based on the distance
    int days = distance / 5;
    String text = "";

    if (days <= 0)
    {
        text = 'Full';
    }
    else if (days == 1)
    {
        text = String(days) + ' day';
    }
    else if (days > 1)
    {
        text = String(days) + ' days';
    }
    else
    {
        text = String(days);
    }

    return days;
}
