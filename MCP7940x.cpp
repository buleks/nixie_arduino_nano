
#include "datetime.h"
#include <Wire.h>
#include <avr/pgmspace.h>

#include "MCP7940x.h"

uint8_t readRegister(uint8_t address, uint8_t offset);
uint8_t readRegister(uint8_t address, uint8_t offset, bool RS);
void writeRegister(uint8_t address, uint8_t offset, uint8_t value);
void writeRegister(uint8_t address, uint8_t offset, uint8_t value, bool RS);
int readRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count);
int readRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count, bool RS);
int readRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count);
int readRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count, bool RS);
int writeRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count);
int writeRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count, bool RS);
int writeRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count);
int writeRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count, bool RS);

void RTC_MCP7940X::oscTrim()
{
    writeRegisterBlock(MCP7940X_ADDRESS, 0x08, 10,
        8); // 127=0111111 = freq- 127ppm
}
void RTC_MCP7940X::adjust(const DateTime& dt)
{
    uint8_t buffer[8];

    buffer[0] = bin2bcd(dt.second()) | 0x80;
    buffer[1] = bin2bcd(dt.minute());
    buffer[2] = bin2bcd(dt.hour());
    buffer[3] = bin2bcd(1 | 0x08);
    buffer[4] = bin2bcd(dt.day());
    buffer[5] = bin2bcd(dt.month());
    buffer[6] = bin2bcd(dt.year() - 2000);
    buffer[7] = 0;

    writeRegisterBlock(MCP7940X_ADDRESS, 0, buffer, 8);
}

DateTime RTC_MCP7940X::now()
{
    uint8_t buffer[7];

    readRegisterBlock(MCP7940X_ADDRESS, 0, buffer, 7);

    uint8_t ss = bcd2bin(buffer[0] & 0x7F);
    uint8_t mm = bcd2bin(buffer[1]);
    uint8_t hh = bcd2bin(buffer[2]);
    uint8_t d = bcd2bin(buffer[4]);
    uint8_t m = bcd2bin(buffer[5] & 0x1F);
    uint16_t y = bcd2bin(buffer[6]) + 2000;

    return DateTime(y, m, d, hh, mm, ss);
}

uint8_t readRegister(uint8_t address, uint8_t offset)
{
    uint8_t data;
    readRegisterBlock(address, offset, &data, 1);
    return (data);
}

uint8_t readRegister(uint8_t address, uint8_t offset, bool RS)
{
    uint8_t data;
    readRegisterBlock(address, offset, &data, 1, RS);
    return (data);
}

int readRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count)
{
    return (readRegisterBlock(address, offset, data, count, true));
}

int readRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count, bool RS)
{

    Wire.beginTransmission(address);
    Wire.write((byte)offset);

    Wire.endTransmission(RS);

    Wire.requestFrom(address, count);

    int ndx = 0;
    unsigned long before = millis();
    while ((ndx < count) && ((millis() - before) < 1000)) {
        if (Wire.available())
            data[ndx++] = Wire.read();
    }
    return ndx;
}

int readRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count)
{
    return (readRegisterBlockW(address, offset, data, count, (bool)true));
}

int readRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count, bool RS)
{

    Wire.beginTransmission(address);
    Wire.write((uint16_t)(offset >> 8));
    Wire.write((uint16_t)(offset & 0xFF));
    Wire.endTransmission(RS);

    Wire.requestFrom(address, count);

    int ndx = 0;
    unsigned long before = millis();
    while ((ndx < count) && ((millis() - before) < 1000)) {
        if (Wire.available())
            data[ndx++] = Wire.read();
    }
    return ndx;
}

void writeRegister(uint8_t address, uint8_t offset, uint8_t value)
{
    writeRegisterBlock(address, offset, &value, 1);
}

void writeRegister(uint8_t address, uint8_t offset, uint8_t value, bool RS)
{
    writeRegisterBlock(address, offset, &value, 1, RS);
}

int writeRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count)
{
    writeRegisterBlock(address, offset, data, count, true);
}

int writeRegisterBlock(uint8_t address, uint8_t offset, uint8_t* data,
    uint8_t count, bool RS)
{

    Wire.beginTransmission(address);
    Wire.write(offset);
    uint8_t ndx;
    for (ndx = 0; ndx < count; ndx++) {
        Wire.write(data[ndx]);
    }
    Wire.endTransmission(RS);

    return ndx;
}

int writeRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count)
{
    return (writeRegisterBlockW(address, offset, data, count, true));
}

int writeRegisterBlockW(uint8_t address, unsigned int offset, uint8_t* data,
    uint8_t count, bool RS)
{

    Wire.beginTransmission(address);
    Wire.write((uint16_t)(offset >> 8));
    Wire.write((uint16_t)(offset & 0xFF));
    uint8_t ndx;
    for (ndx = 0; ndx < count; ndx++) {
        Wire.write(data[ndx]);
    }

    Wire.endTransmission(RS);

    return ndx;
}
