#include "main.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Error: expected exactly 2 command-line arguments." << std::endl;
        std::cerr << "Usage: " << argv[0] << " <loop_bound> <increment>" << std::endl;
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

    std::cout << "Loop bound bits:" << std::endl;
    std::cout << loopBoundBits.formatted() << std::endl;
    std::cout << "Increment bits:" << std::endl;
    std::cout << incrementBits.formatted() << std::endl;

    const int floorLog2Increment = floorLog2FromBits(incrementBits);
    const bool incrementIsPowerOfTwo = isPowerOfTwoFloatBits(incrementBits);

    const int thresholdExponent = floorLog2Increment + 24 + (incrementIsPowerOfTwo ? 0 : 1);

    float thresholdValue = 0.0F;
    Float32Bits thresholdBits(0.0F);
    const bool hasFiniteThreshold = buildPowerOfTwoFromExponent(thresholdExponent, thresholdValue, thresholdBits);

    if (!hasFiniteThreshold || loopBound < thresholdValue)
    {
        std::cout << "No overflow!" << std::endl;
        return 0;
    }

    std::cout << "Warning: Possible overflow!" << std::endl;
    std::cout << "Overflow threshold:" << std::endl;
    std::cout << std::scientific << std::setprecision(9) << thresholdValue << std::endl;
    std::cout << thresholdBits.formatted() << std::endl;

    return 0;
}
