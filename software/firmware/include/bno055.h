#pragma once
#include <Arduino.h>
class bno55
{
public:
    bno55(uint8_t i2cAddr);
    ~bno55();
    void init();
};