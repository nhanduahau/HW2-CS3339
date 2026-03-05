/*
 * File: fp_utils.cpp
 * Course: CS3339, Spring 2026
 * Assignment: Homework 2 - Binary Representation, Floating-point Math and Bit Manipulation
 * Student: Ly Huu Nhan To (Harry)
 * Description: Implementations for float bit conversion and overflow-threshold computations.
 */

#include "fp_utils.h"

Float32Bits::Float32Bits(float value) : value_(value), bits_(floatToBits(value))
{
    /*
     * Function: Float32Bits::Float32Bits(float)
     * Purpose: Construct from a float value and compute its raw IEEE-754 bits.
     * Parameters:
     *   value - input float value
     * Returns:
     *   na
     * Exceptions:
     *   none
     */
}

Float32Bits::Float32Bits(uint32_t bits) : value_(bitsToFloat(bits)), bits_(bits)
{
    /*
     * Function: Float32Bits::Float32Bits(uint32_t)
     * Purpose: Construct from raw IEEE-754 bits and compute the float value.
     * Parameters:
     *   bits - 32-bit binary representation
     * Returns:
     *   na
     * Exceptions:
     *   none
     */
}

float Float32Bits::value() const
{
    /*
     * Function: Float32Bits::value
     * Purpose: Return the stored float value.
     * Parameters:
     *   none
     * Returns:
     *   stored float value
     * Exceptions:
     *   none
     */
    return value_;
}

uint32_t Float32Bits::bits() const
{
    /*
     * Function: Float32Bits::bits
     * Purpose: Return the stored 32-bit IEEE-754 representation.
     * Parameters:
     *   none
     * Returns:
     *   stored 32-bit bit pattern
     * Exceptions:
     *   none
     */
    return bits_;
}

std::string Float32Bits::spaced() const
{
    /*
     * Function: Float32Bits::spaced
     * Purpose: Format bits as "sign exponent fraction" with space separators.
     * Parameters:
     *   none
     * Returns:
     *   formatted binary32 string
     * Exceptions:
     *   none
     */
    std::bitset<32> b(bits_);
    const std::string s = b.to_string();
    return s.substr(0, 1) + " " + s.substr(1, 8) + " " + s.substr(9, 23);
}

int Float32Bits::exponentField() const
{
    /*
     * Function: Float32Bits::exponentField
     * Purpose: Extract the 8-bit exponent field from the stored bits.
     * Parameters:
     *   none
     * Returns:
     *   exponent field value as int
     * Exceptions:
     *   none
     */
    return static_cast<int>((bits_ >> 23U) & 0xFFU);
}

uint32_t Float32Bits::fractionField() const
{
    /*
     * Function: Float32Bits::fractionField
     * Purpose: Extract the 23-bit fraction field from the stored bits.
     * Parameters:
     *   none
     * Returns:
     *   fraction field value as uint32_t
     * Exceptions:
     *   none
     */
    return bits_ & 0x7FFFFFU;
}

uint32_t Float32Bits::floatToBits(float value)
{
    /*
     * Function: Float32Bits::floatToBits
     * Purpose: Convert float to raw IEEE-754 binary32 bits.
     * Parameters:
     *   value - input float value
     * Returns:
     *   uint32_t bit pattern of value
     * Exceptions:
     *   none
     */
    uint32_t bits = 0U;
    std::memcpy(&bits, &value, sizeof(float));
    return bits;
}

float Float32Bits::bitsToFloat(uint32_t bits)
{
    /*
     * Function: Float32Bits::bitsToFloat
     * Purpose: Convert raw IEEE-754 binary32 bits to float.
     * Parameters:
     *   bits - 32-bit float representation
     * Returns:
     *   float value decoded from bits
     * Exceptions:
     *   none
     */
    float value = 0.0F;
    std::memcpy(&value, &bits, sizeof(float));
    return value;
}

bool parsePositiveFloat(const char *text, float &outValue)
{
    /*
     * Function: parsePositiveFloat
     * Purpose: Parse and validate a positive finite floating-point command-line input.
     * Parameters:
     *   text - C-string input token
     *   outValue - parsed float output
     * Returns:
     *   true if valid positive finite float, otherwise false
     * Exceptions:
     *   none
     */
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
    /*
     * Function: highestSetBitPosition
     * Purpose: Find index of highest set bit in a 32-bit unsigned value.
     * Parameters:
     *   value - input bit pattern
     * Returns:
     *   highest set-bit index, or -1 if value is zero
     * Exceptions:
     *   none
     */
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
    /*
     * Function: isPowerOfTwoFloatBits
     * Purpose: Determine whether a binary32 value represents an exact power of two.
     * Parameters:
     *   valueBits - wrapped float and fields
     * Returns:
     *   true when value is an exact power of two, otherwise false
     * Exceptions:
     *   none
     */
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
    /*
     * Function: floorLog2FromBits
     * Purpose: Compute floor(log2(x)) directly from binary32 exponent/fraction fields.
     * Parameters:
     *   valueBits - wrapped positive float and bit fields
     * Returns:
     *   floor base-2 exponent
     * Exceptions:
     *   none
     */
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
    /*
     * Function: buildPowerOfTwoFromExponent
     * Purpose: Build the representable binary32 value 2^exponent from bit fields.
     * Parameters:
     *   exponent - target power-of-two exponent
     *   valueOut - output float value
     *   bitsOut - output wrapped bit/value object
     * Returns:
     *   true if representable as finite binary32, otherwise false
     * Exceptions:
     *   none
     */
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
