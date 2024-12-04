#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

typedef unsigned long long ull;

// Function to calculate the number of ways without using a loop
ull numWays(ull time, ull distance) {
    // Handle edge cases
    if (time == 0) return 0;

    // Compute the discriminant
    long double discriminant = (long double)time * time - 4.0 * (long double)distance;

    // If discriminant is negative, check if the quadratic is always negative
    if (discriminant < 0) {
        // Since the coefficient of i^2 is positive, the quadratic is always positive
        // Therefore, no solutions
        return 0;
    }

    // Calculate square root of discriminant
    long double sqrtD = sqrt((long double)discriminant);

    // Calculate the two roots
    long double i1 = ((long double)time - sqrtD) / 2.0;
    long double i2 = ((long double)time + sqrtD) / 2.0;

    // Ceiling of i1 (smallest integer greater than i1)
    ull lower = std::ceil(i1);

    // Floor of i2 (largest integer less than i2)
    ull upper = std::floor(i2);

    // Clamp the values within [1, time]
    lower = std::max(lower, (ull)1);
    upper = std::min(upper, time);

    // If lower > upper, no solutions
    if (lower > upper) return 0;

    // The number of integer solutions is upper - lower + 1
    return upper - lower + 1;
}
int main() {
    std::string timeLine, distanceLine;   
 
    std::getline(std::cin, timeLine);
    std::getline(std::cin, distanceLine);

    timeLine.erase(0, timeLine.find(":") + 1);
    timeLine.erase(remove(timeLine.begin(), timeLine.end(), ' '), timeLine.end());

    distanceLine.erase(0, distanceLine.find(":") + 1);
    distanceLine.erase(remove(distanceLine.begin(), distanceLine.end(), ' '), distanceLine.end());

    ull time = std::stoull(timeLine);
    ull distance = std::stoull(distanceLine);

    std::cout << numWays(time, distance) << std::endl;
    
    return 0;
}
