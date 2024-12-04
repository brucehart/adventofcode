#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int numWays(int time, int distance)
{
    int count = 0;

    for(int i=1; i<=time; i++)
    {
        int d = i*(time - i);
        if (d > distance) count++;        
    }

    return count;
}

int main() {
    std::string timeLine, distanceLine;
   
    std::vector<int> times;
    std::vector<int> distances;

    std::getline(std::cin, timeLine);
    std::getline(std::cin, distanceLine);

    // Parse the Time line
    std::istringstream timeStream(timeLine);
    std::string label;
    timeStream >> label; // Skip the "Time:" label
    int t;
    while (timeStream >> t) {
        times.push_back(t);        
    }

    // Parse the Distance line
    std::istringstream distanceStream(distanceLine);
    distanceStream >> label; // Skip the "Distance:" label
    int d;
    while (distanceStream >> d) {
        distances.push_back(d);
    }

    int marginOfError = 1;

    for(int i=0; i<times.size(); i++)
    {        
        marginOfError *= numWays(times[i], distances[i]);
    }
    
    std::cout << marginOfError << std::endl;
    
    return 0;
}
