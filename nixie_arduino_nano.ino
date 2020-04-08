#include "MCP7940x.h"
#include "datetime.h"
#include "nixie.h"
#include <Wire.h>
RTC_MCP7940X RTC;

int neonState = LOW;

const int SW1 = A2;
const int SW2 = A6;
const int NEON = A3;

void setup()
{

    showZero();
    initNixiePins();
    pinMode(NEON, OUTPUT);
    pinMode(SW1, INPUT_PULLUP);
    pinMode(SW2, INPUT_PULLUP);
    Wire.begin();

    RTC.oscTrim();

    //  RTC.adjust(DateTime(__DATE__, __TIME__));
}

bool sw1State = 0;
bool sw2State = 0;
bool sw1Pressed = 0;
bool sw2Pressed = 0;
void checkButtons()
{
    static bool sw1LastState = 0;
    static bool sw2LastState = 0;
    sw1State = 0;
    sw2State = 0;

    if (digitalRead(SW1) == LOW) {

        if (sw1LastState == 0) {
            sw1Pressed = 1;
        }
        if (sw1LastState == 1) {
            sw1State = 1;
        }
        sw1LastState = 1;
    } else {
        sw1LastState = 0;
    }
    if (analogRead(SW2) < 128) {

        if (sw2LastState == 0) {
            sw2Pressed = 1;
        }
        if (sw2LastState == 1) {
            sw2State = 1;
        }
        sw2LastState = 1;
    } else {
        sw2LastState = 0;
    }
}

bool isSW1Pressed()
{
    if (sw1Pressed) {
        sw1Pressed = 0;
        return 1;
    }
    return 0;
}

bool isSW2Pressed()
{
    if (sw2Pressed) {
        sw2Pressed = 0;
        return 1;
    }
    return 0;
}

void increaseHours(DateTime& time)
{
    time.hh += 1;
    time.ss = 0;
    if (time.hh > 24) {
        time.hh = 0;
    }
    RTC.adjust(time);
    updateDisplay(&time);
}

void increaseMinutes(DateTime& time)
{
    time.mm += 1;
    time.ss = 0;
    if (time.mm >= 60) {
        time.mm = 0;
    }
    RTC.adjust(time);
    updateDisplay(&time);
}
void loop()
{
    static unsigned long rtcRead = 2000;
    static unsigned long previousMillis2 = 0;
    static unsigned long previousRefresh = millis();
    static unsigned long checkButton = 0;
    static unsigned long nixieUpdate = 0;
    static unsigned long previousTimeset = 0;
    static bool timeready = 0;

    static DateTime time;

    if (millis() - rtcRead >= 5000) {
        rtcRead = millis();
        time = RTC.now();
        timeready = 1;
    }
    if (millis() - nixieUpdate >= 1000 && timeready == 1) {
        nixieUpdate = millis();
        updateDisplay(&time);
    }

    if (millis() - previousMillis2 >= 1000) {
        previousMillis2 = millis();
        if (neonState == LOW) {
            neonState = HIGH;
        } else {
            neonState = LOW;
        }
        digitalWrite(NEON, neonState);
    }

    if (millis() - previousRefresh >= 30000) {
        previousRefresh = millis();
        refreshNixie();
    }

    if (millis() - checkButton >= 1) {
        checkButtons();
        checkButton = millis();
    }

    static int updatePeriod = 500;
    static int pressedCounter = 0;
    if (isSW1Pressed()) {
        increaseMinutes(time);
        previousTimeset = millis();
    } else if (isSW2Pressed()) {
        increaseHours(time);
        previousTimeset = millis();
    } else if (millis() - previousTimeset >= updatePeriod) {
        previousTimeset = millis();

        if (pressedCounter > 2) {
            updatePeriod = 100;
        } else {
            updatePeriod = 500;
        }

        if (sw2State) {
            pressedCounter++;
            increaseHours(time);

        } else if (sw1State) {
            pressedCounter++;
            increaseMinutes(time);
        } else {
            pressedCounter = 0;
        }
    }
}
