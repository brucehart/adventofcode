#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string power;
    int lineCount = 0;
    std::vector<int> gamma_count;    
    int gamma = 0, epsilon = 0;   
        
    while (std::cin >> power)
    {
        if (gamma_count.size() < power.size()) gamma_count.resize(power.size());
        for (int i = 0; i < power.size(); i++) gamma_count[i] += (power[i]=='1') ? 1 : 0;;
        lineCount++;        
    }

    for(auto g: gamma_count)
    {
        gamma *= 2;
        epsilon *= 2;

        if (g > lineCount/2)
            gamma += 1;                    
        else
            epsilon += 1;            
    }

    std::cout << gamma*epsilon << std::endl;

}