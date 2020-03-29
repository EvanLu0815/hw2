#include "mbed.h"


BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);

char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char table1[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
Serial pc(USBTX, USBRX);

int sample = 1024;
int i;
float j;
float ADCdata[1024];
int f = 0;
int a[3];

int main()
{
    greenLED = 1;
    redLED = 1;
    Aout = 0;

    for (i = 0; i < sample; i++) {
        ADCdata[i] = Ain;
        wait(1.0 / sample);
    }
    for (i = 1; i < sample; i++) {
        if (ADCdata[i - 1] > 0.3 && ADCdata[i] <= 0.3) {
            f++;
        }
    }
    a[0] = f % 1000 / 100;
    a[1] = (f % 100) / 10;
    a[2] = f % 10;
    for (i = 0; i < sample; i++) {
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }
    while (1) {
        if (!Switch) {
            Aout = 0;
            greenLED = 1;
            redLED = 0;
            display = table[a[0]];
            wait(0.5);
            display = table[a[1]];
            wait(0.5);
            display = table1[a[2]];
            wait(0.5);
        }
        else {
            greenLED = 0;
            redLED = 1;
            display = 0;
            for (i = 0; i < 1; i++) {
                for (j = 0; j < 2; j += 1.0 / sample) {
                    Aout = 0.5 + 0.5 * sin(f * j * 2 * 3.14159);
                    wait(1.0/sample);
                }
            }
        }
    }
}