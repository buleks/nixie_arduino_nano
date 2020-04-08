#ifndef NIXIE___
#define NIXIE___

#include "datetime.h"

void initNixiePins();
void setMinuteUnity(int value);
void setMinuteTens(int value);
void setHourUnity(int value);
void setHourTens(int value);
void showZero();
void showNines();
void refreshNixie();
void updateDisplay(DateTime* time);
#endif
