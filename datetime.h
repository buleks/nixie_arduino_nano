#ifndef DATETIME____
#define DATETIME____
#include <inttypes.h>

class DateTime {
public:
    DateTime();
    DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0,
        uint8_t min = 0, uint8_t sec = 0);
    DateTime(const char* date, const char* time);

    uint16_t year() const { return 2000 + yOff; }
    uint8_t month() const { return m; }
    uint8_t day() const { return d; }
    uint8_t hour() const { return hh; }
    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }

    uint8_t yOff, m, d, hh, mm, ss;
};

#endif
