#pragma once

#include <bitset>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

class Float32Bits
{
public:
    explicit Float32Bits(float value);
    explicit Float32Bits(uint32_t bits);

    float value() const;
    uint32_t bits() const;
    std::string formatted() const;
    int exponentField() const;
    uint32_t fractionField() const;

    static uint32_t floatToBits(float value);
    static float bitsToFloat(uint32_t bits);

private:
    float value_;
    uint32_t bits_;
};

bool parsePositiveFloat(const char *text, float &outValue);
int highestSetBitPosition(uint32_t value);
bool isPowerOfTwoFloatBits(const Float32Bits &valueBits);
int floorLog2FromBits(const Float32Bits &valueBits);
bool buildPowerOfTwoFromExponent(int exponent, float &valueOut, Float32Bits &bitsOut);
