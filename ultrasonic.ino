#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

#define PIN_TRIG 13
#define PIN_ECHO 12
#define PIN_LED 3

void setup()
{

    // initialize ultrasonic
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    // initialize LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // initialize lcd screen
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.print("Ultrasonic v1.0");
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

    int duration = pulseIn(PIN_ECHO, HIGH);

    // Use 343 metres per second as speed of sound
    float distance = duration * 0.034 / 2;

    if (distance <= 10)
    {
        digitalWrite(PIN_LED, HIGH);
    }
    else
    {
        digitalWrite(PIN_LED, LOW);
    }

    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hladina: ");
    lcd.print(distance);
    lcd.print("cm");

    lcd.setCursor(0, 1);
    lcd.print("Zbyva: ");
    lcd.print(getDays(distance));
    lcd.print(" den");

    delay(3000);
    lcd.clear();
}

int getDays(int distance)
{
    // Calculate the number of days based on the distance
    int days = distance / 5;

    return days;
}
