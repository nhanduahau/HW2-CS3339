/*
 * File: main.cpp
 * Course: CS3339, Spring 2026
 * Assignment: Homework 2 - Binary Representation, Floating-point Math and Bit Manipulation
 * Student: Ly Huu Nhan To (Harry)
 * Description: Driver file for floating-point increment-loss overflow detection.
 */

#include "main.h"

int main(int argc, char *argv[])
{
    /*
     * Function: main
     * Purpose: Parse command-line input, print IEEE-754 bit patterns, and report overflow risk.
     * Parameters:
     *   argc - number of command-line arguments
     *   argv - array of command-line argument strings
     * Returns:
     *   0 on success, 1 on invalid input/usage
     * Exceptions:
     *   none
     */
    if (argc != 3)
    {
        std::cerr << "usage:" << std::endl;
        std::cerr << "    ./fp_overflow_checker loop_bound loop_counter" << std::endl;
        std::cerr << std::endl;
        std::cerr << "    loop_bound is a positive floating-point value" << std::endl;
        std::cerr << "    loop_counter is a positive floating-point value" << std::endl;
        return 1;
    }

    float loopBound = 0.0F;
    float increment = 0.0F;

    if (!parsePositiveFloat(argv[1], loopBound))
    {
        std::cerr << "Error: invalid loop_bound (must be a positive finite float)." << std::endl;
        return 1;
    }

    if (!parsePositiveFloat(argv[2], increment))
    {
        std::cerr << "Error: invalid increment (must be a positive finite float)." << std::endl;
        return 1;
    }

    const Float32Bits loopBoundBits(loopBound);
    const Float32Bits incrementBits(increment);

    std::cout << std::left << std::setw(14) << "Loop bound:" << loopBoundBits.spaced() << "\n";
    std::cout << std::left << std::setw(14) << "Loop counter:" << incrementBits.spaced() << "\n";

    const int floorLog2Increment = floorLog2FromBits(incrementBits);
    const bool incrementIsPowerOfTwo = isPowerOfTwoFloatBits(incrementBits);

    const int thresholdExponent = floorLog2Increment + 24 + (incrementIsPowerOfTwo ? 0 : 1);

    float thresholdValue = 0.0F;
    Float32Bits thresholdBits(0.0F);
    const bool hasFiniteThreshold = buildPowerOfTwoFromExponent(thresholdExponent, thresholdValue, thresholdBits);

    if (!hasFiniteThreshold || loopBound < thresholdValue)
    {
        std::cout << "There is no overflow!" << std::endl;
        return 0;
    }

    std::cout << "Warning: Possible overflow!" << std::endl;
    std::cout << "Overflow threshold:" << std::endl;
    std::cout << "\t" << thresholdValue << std::endl;
    std::cout << "\t" << thresholdBits.spaced() << std::endl;

    return 0;
}
