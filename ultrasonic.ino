#include <OneWire.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header
#include <DallasTemperature.h>

/**
 * Test Arduino UNO with ultrasonic sensor
 * Schema: https://wokwi.com/projects/395127928831719425
 */

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

#define PIN_ECHO 12
#define PIN_TRIG 11
#define PIN_LED 3
#define PIN_TEMP 2

float distance = 0;

OneWire oneWire(PIN_TEMP);           // Inicializace OneWire sběrnice na daném pinu
DallasTemperature sensors(&oneWire); // Inicializace čidla DallasTemperature

void setup()
{
    // initialize temperature sensor
    sensors.begin();

    // initialize ultrasonic
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    // initialize LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // initialize lcd screen
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.print("Ultrasonic v1.6");
    delay(1000);
    lcd.clear();
    lcd.noBacklight();
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

        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print(distance);
        lcd.print("cm, ");
        lcd.print(temperature);
        lcd.print((char)223);
        lcd.print("C");

        lcd.setCursor(0, 1);
        lcd.print("Days: ");
        lcd.print(getDays(distance));

        delay(3000);
    }

    else
    {
        // Turn off the LCD backlight
        lcd.noBacklight();
        lcd.clear();
    }
}

int getDays(int distance)
{
    // Calculate the number of days based on the distance
    int days = distance / 5;

    return days;
}
