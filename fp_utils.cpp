#include "fp_utils.h"

Float32Bits::Float32Bits(float value) : value_(value), bits_(floatToBits(value)) {}

Float32Bits::Float32Bits(uint32_t bits) : value_(bitsToFloat(bits)), bits_(bits) {}

float Float32Bits::value() const
{
    return value_;
}

uint32_t Float32Bits::bits() const
{
    return bits_;
}

std::string Float32Bits::formatted() const
{
    std::bitset<32> b(bits_);
    const std::string s = b.to_string();
    return s.substr(0, 1) + " | " + s.substr(1, 8) + " | " + s.substr(9, 23);
}

int Float32Bits::exponentField() const
{
    return static_cast<int>((bits_ >> 23U) & 0xFFU);
}

uint32_t Float32Bits::fractionField() const
{
    return bits_ & 0x7FFFFFU;
}

uint32_t Float32Bits::floatToBits(float value)
{
    uint32_t bits = 0U;
    std::memcpy(&bits, &value, sizeof(float));
    return bits;
}

float Float32Bits::bitsToFloat(uint32_t bits)
{
    float value = 0.0F;
    std::memcpy(&value, &bits, sizeof(float));
    return value;
}

bool parsePositiveFloat(const char *text, float &outValue)
{
    char *endPtr = nullptr;
    outValue = std::strtof(text, &endPtr);

    if (endPtr == text || *endPtr != '\0')
    {
        return false;
    }

    if (!std::isfinite(outValue) || outValue <= 0.0F)
    {
        return false;
    }

    return true;
}

int highestSetBitPosition(uint32_t value)
{
    for (int bit = 31; bit >= 0; --bit)
    {
        if ((value & (1U << bit)) != 0U)
        {
            return bit;
        }
    }
    return -1;
}

bool isPowerOfTwoFloatBits(const Float32Bits &valueBits)
{
    const int exp = valueBits.exponentField();
    const uint32_t frac = valueBits.fractionField();

    if (exp == 0)
    {
        return frac != 0U && (frac & (frac - 1U)) == 0U;
    }

    return frac == 0U;
}

int floorLog2FromBits(const Float32Bits &valueBits)
{
    const int exp = valueBits.exponentField();
    const uint32_t frac = valueBits.fractionField();

    if (exp != 0)
    {
        return exp - 127;
    }

    const int msb = highestSetBitPosition(frac);
    return msb - 149;
}

bool buildPowerOfTwoFromExponent(int exponent, float &valueOut, Float32Bits &bitsOut)
{
    uint32_t bits = 0U;

    if (exponent > 127)
    {
        return false;
    }

    if (exponent >= -126)
    {
        bits = static_cast<uint32_t>(exponent + 127) << 23U;
    }
    else if (exponent >= -149)
    {
        const int fracBit = exponent + 149;
        bits = 1U << fracBit;
    }
    else
    {
        return false;
    }

    bitsOut = Float32Bits(bits);
    valueOut = bitsOut.value();
    return true;
}
