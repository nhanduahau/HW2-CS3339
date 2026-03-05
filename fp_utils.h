/*
 * File: fp_utils.h
 * Course: CS3339, Spring 2026
 * Assignment: Homework 2 - Binary Representation, Floating-point Math and Bit Manipulation
 * Student: Ly Huu Nhan To (Harry)
 * Description: Declarations for IEEE-754 float bit utilities and overflow-threshold helpers.
 */

#ifndef FP_UTILS_H
#define FP_UTILS_H

#include <bitset>
#include <cmath>
#include <cstdint>
#include <cstring>

/*
 * Class: Float32Bits
 * Purpose: Store and expose float value plus its IEEE-754 binary32 bit representation.
 */
class Float32Bits
{
public:
    explicit Float32Bits(float value);
    explicit Float32Bits(uint32_t bits);

    float value() const;
    uint32_t bits() const;
    std::string spaced() const;
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

#endif // FP_UTILS_H
