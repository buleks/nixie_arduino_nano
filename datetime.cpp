#include "datetime.h"

static uint8_t conv2d(const char* p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

DateTime::DateTime()
{
    ss = 0;
    mm = 47;
    hh = 21;
    yOff = 20;
    m = 4;
    d = 1;
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
    uint8_t min, uint8_t sec)
{
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
DateTime::DateTime(const char* date, const char* time)
{
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
    case 'J':
        m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7;
        break;
    case 'F':
        m = 2;
        break;
    case 'A':
        m = date[2] == 'r' ? 4 : 8;
        break;
    case 'M':
        m = date[2] == 'r' ? 3 : 5;
        break;
    case 'S':
        m = 9;
        break;
    case 'O':
        m = 10;
        break;
    case 'N':
        m = 11;
        break;
    case 'D':
        m = 12;
        break;
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);
}
