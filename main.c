#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define HIGH    0
#define LOW     1

#define led_red     26
#define led_green   13
#define led_blue    5

#define button_green    21
#define button_blue     18
#define button_red      23

void buttonInitialise(int pin)
{
    gpioSetMode(pin, PI_INPUT);
    gpioSetPullUpDown(pin, PI_PUD_UP);
}

void buttonsInitialise()
{
    buttonInitialise(button_green);
    buttonInitialise(button_blue);
    buttonInitialise(button_red);
}

void ledInitialise()
{
    gpioSetPWMrange(led_red, 100);
    gpioSetPWMrange(led_blue, 100);
    gpioSetPWMrange(led_green, 100);
}

void setColor(int r, int g, int b)
{
    gpioPWM(led_red, r);
    gpioPWM(led_green, g);
    gpioPWM(led_blue, b);
}

unsigned long millis() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec * 1000) + (now.tv_usec / 1000);
}

int main()
{
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    ledInitialise();
    buttonsInitialise();

    int readingButtonRed;
    int readingButtonGreen;
    int readingButtonBlue;

    int buttonStateRed      = LOW;
    int buttonStateGreen    = LOW;
    int buttonStateBlue     = LOW;

    int lastButtonStateRed      = HIGH;
    int lastButtonStateGreen    = HIGH;
    int lastButtonStateBlue     = HIGH;

    long lastUpdateTimeButtonRed;
    long lastUpdateTimeButtonGreen;
    long lastUpdateTimeButtonBlue;

    long captureTime = 50;

    while(1) {
        readingButtonRed    = gpioRead(button_red);
        readingButtonGreen  = gpioRead(button_green);
        readingButtonBlue   = gpioRead(button_blue);

        if (readingButtonRed != lastButtonStateRed) {
            fprintf(stdout, "Button RED state update \n");
            lastUpdateTimeButtonRed = millis();
        }

        if (readingButtonGreen != lastButtonStateGreen) {
            fprintf(stdout, "Button GREEN state update \n");
            lastUpdateTimeButtonGreen = millis();
        }

        if (readingButtonBlue != lastButtonStateBlue) {
           fprintf(stdout, "Button BLUE state update \n");
           lastUpdateTimeButtonBlue = millis();
        }

        if ((millis() - lastUpdateTimeButtonRed) > captureTime && readingButtonRed != buttonStateRed) {
           fprintf(stdout, "Button RED pressed \n");
           buttonStateRed = readingButtonRed;

           if (HIGH == buttonStateRed) {
               fprintf(stdout, "Button RED set color \n");
               setColor(0, 100, 100);
           }
        }

        if ((millis() - lastUpdateTimeButtonGreen) > captureTime && readingButtonGreen != buttonStateGreen) {
           fprintf(stdout, "Button GREEN pressed \n");
           buttonStateGreen = readingButtonGreen;

           if (HIGH == buttonStateGreen) {
               fprintf(stdout, "Button GREEN set color \n");
               setColor(100, 0, 100);
           }
        }

        if ((millis() - lastUpdateTimeButtonBlue) > captureTime && readingButtonBlue != buttonStateBlue) {
             fprintf(stdout, "Button BLUE pressed \n");
           buttonStateBlue = readingButtonBlue;

           if (HIGH == buttonStateBlue) {
               fprintf(stdout, "Button BLUE set color \n");
               setColor(100, 100, 0);
           }
        }

        lastButtonStateRed      = readingButtonRed;
        lastButtonStateGreen    = readingButtonGreen;
        lastButtonStateBlue     = readingButtonBlue;
    }

    return 0;
}