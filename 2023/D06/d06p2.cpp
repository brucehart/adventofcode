#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

typedef unsigned long long ull;

ull numWays(ull time, ull distance)
{
    ull count = 0;

    for(ull i=1; i<=time; i++)
    {
        ull d = i*(time - i);
        if (d > distance) count++;        
    }

    return count;
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
