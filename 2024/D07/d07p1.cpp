#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

// Define the type for unsigned long long
typedef unsigned long long ull;

// Define the Equation struct
struct Equation {
    enum operators {MULTIPLY, ADD};
    ull sum;
    std::vector<ull> elements;
};

bool solveEquation(Equation eq, ull runningSum = 0, int startIdx = 0, Equation::operators op = Equation::ADD) {
    // Base case: if we have reached the end of the elements
    if (startIdx == eq.elements.size()) {
        return runningSum == eq.sum;
    }

    auto nextElement = [&](ull rSum) -> bool {
        return solveEquation(eq, rSum, startIdx + 1, Equation::ADD) ||
               solveEquation(eq, rSum, startIdx + 1, Equation::MULTIPLY);
    };
    
    if (startIdx == 0) {
        if (startIdx > runningSum) return false;
        return nextElement(eq.elements[startIdx]);
    }

    if (op == Equation::ADD)
        runningSum += eq.elements[startIdx];
    else
        runningSum *= eq.elements[startIdx];

    if (runningSum > eq.sum) return false;

    return nextElement(runningSum);    
}

int main() {
    std::vector<Equation> equations;
    std::string line;

    // Read from standard input line by line
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        Equation eq;

        // Read the sum (before the colon)
        std::string sumStr;
        std::getline(iss, sumStr, ':');
        eq.sum = std::stoull(sumStr);

        // Read the elements (after the colon)
        ull element;
        while (iss >> element) {
            eq.elements.push_back(element);
        }

        // Store the Equation object in the vector
        equations.push_back(eq);
    }

    ull totalSum = 0;
    
    std::for_each(equations.begin(), equations.end(), [&totalSum](Equation eq) {
        if (solveEquation(eq))
            totalSum += eq.sum;
    });

    std::cout << totalSum << std::endl;
    return 0;
}
