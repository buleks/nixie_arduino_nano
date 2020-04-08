#ifndef MCP7940X
#define MCP7940X
#include "Arduino.h"
#include "datetime.h"
#include <inttypes.h>

#define MCP7940X_ADDRESS 0x6F

class RTC_MCP7940X {
public:
    static void adjust(const DateTime& dt);
    static DateTime now();

    static DateTime GetPwrOn();

    void oscTrim();
    static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
    static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }
};

#endif
