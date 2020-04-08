#include "nixie.h"
#include <Arduino.h>

const int MINUTE1[4] = { 13, 12, A1, A0 };
const int MINUTE2[4] = { 11, 8, 9, 10 };
const int HOUR2[4] = { 3, 1, 0, 2 };
const int HOUR1[4] = { 7, 4, 5, 6 };

byte BCD[12][4] = { { 1, 0, 0, 1 }, { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 0, 1, 0, 0 },
    { 1, 1, 0, 0 }, { 0, 0, 1, 0 }, { 1, 0, 1, 0 }, { 0, 1, 1, 0 },
    { 1, 1, 1, 0 }, { 0, 0, 0, 1 },

    { 0, 0, 0, 0 }, { 1, 1, 1, 1 } };

void initNixiePins()
{
    for (int i = 0; i < 4; i++) {
        pinMode(MINUTE1[i], OUTPUT); // set as outputs
        pinMode(MINUTE2[i], OUTPUT);
        pinMode(HOUR1[i], OUTPUT);
        pinMode(HOUR2[i], OUTPUT);
    }
}

void setMinuteUnity(int value)
{

    if (value > 10) // if too high turn off
        value = 11;

    for (int n = 0; n < 4; n++) {
        digitalWrite(MINUTE1[n], BCD[value][n]);
    }
}
void setMinuteTens(int value)
{

    if (value > 10)
        value = 11;
    for (int n = 0; n < 4; n++) {
        digitalWrite(MINUTE2[n], BCD[value][n]);
    }
}
void setHourUnity(int value)
{

    if (value > 10)
        value = 11;
    for (int n = 0; n < 4; n++) {
        digitalWrite(HOUR1[n], BCD[value][n]);
    }
}
void setHourTens(int value)
{

    if (value > 10)
        value = 12;
    for (int n = 0; n < 4; n++) {
        digitalWrite(HOUR2[n], BCD[value][n]);
    }
}
void showZero()
{
    setMinuteUnity(0);
    setMinuteTens(0);
    setHourUnity(0);
    setHourTens(0);
}
void showNines()
{
    setMinuteUnity(9);
    setMinuteTens(9);
    setHourUnity(9);
    setHourTens(9);
}

void refreshNixie()
{
    for (int i = 0; i < 10; i++) {
        setMinuteUnity(i);
        setMinuteTens(i);
        setHourUnity(i);
        setHourTens(i);
        delay(50);
    }
}

void updateDisplay(DateTime* time)
{
    int minute = 0;
    int hour = 0;

    hour = time->hour();
    if (hour == 24) {
        hour = 0;
    }
    minute = time->minute();
    setMinuteUnity(minute % 10);
    setMinuteTens((int)((minute / 10) % 10));
    setHourUnity(hour % 10);
    setHourTens((int)(hour / 10) % 10);
}
