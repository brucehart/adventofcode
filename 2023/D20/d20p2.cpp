#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <numeric>

// Structure to represent a pulse
struct Pulse {
    std::string from;
    std::string to;
    bool isHigh;
};

// Structure to represent a module
struct Module {
    char type;
    std::vector<std::string> outputs;
    // For flip-flops:
    bool isOn = false;
    // For conjunctions:
    std::map<std::string, bool> memory;
};

// Function to calculate the least common multiple (LCM)
long long lcm(long long a, long long b) {
    return (a * b) / std::gcd(a, b);
}

int main() {
    std::map<std::string, Module> modules;
    std::string line;

    // Read input and build the module map
    while (std::getline(std::cin, line)) {
        std::string name = line.substr(0, line.find(" -> "));
        std::string outputs_str = line.substr(line.find(" -> ") + 4);
        std::vector<std::string> outputs;
        size_t pos = 0;
        std::string token;
        while ((pos = outputs_str.find(", ")) != std::string::npos) {
            token = outputs_str.substr(0, pos);
            outputs.push_back(token);
            outputs_str.erase(0, pos + 2);
        }
        outputs.push_back(outputs_str);

        if (name == "broadcaster") {
            modules[name] = {'b', outputs};
        } else {
            char type = name[0];
            name = name.substr(1);
            modules[name] = {type, outputs};
        }
    }

    // Initialize conjunction modules' memory
    for (auto& [name, module] : modules) {
        for (const std::string& output : module.outputs) {
            if (modules.count(output) && modules[output].type == '&') {
                modules[output].memory[name] = false;
            }
        }
    }

    // Find the module that outputs to 'rx' (should be a conjunction)
    std::string rx_input;
    for (const auto& [name, module] : modules) {
        if (std::find(module.outputs.begin(), module.outputs.end(), "rx") != module.outputs.end()) {
            rx_input = name;
            break;
        }
    }

    // Find the modules that feed into the 'rx_input' conjunction
    std::map<std::string, long long> cycle_lengths;
    for (const auto& [name, module] : modules) {
        if (std::find(module.outputs.begin(), module.outputs.end(), rx_input) != module.outputs.end()) {
            cycle_lengths[name] = 0;
        }
    }
    
    // Simulate button presses until all cycle lengths are found
    long long buttonPresses = 0;
    while (std::any_of(cycle_lengths.begin(), cycle_lengths.end(), [](auto p){return p.second == 0;})) {
        buttonPresses++;
        std::queue<Pulse> q;
        q.push({"button", "broadcaster", false});

        while (!q.empty()) {
            Pulse pulse = q.front();
            q.pop();

            if (pulse.to == rx_input && pulse.isHigh) {
                if (cycle_lengths.count(pulse.from) > 0 && cycle_lengths[pulse.from] == 0)
                {
                    cycle_lengths[pulse.from] = buttonPresses;
                }
            }
            
            if (!modules.count(pulse.to)) continue;

            Module& module = modules[pulse.to];

            if (module.type == 'b') {
                for (const std::string& output : module.outputs) {
                    q.push({pulse.to, output, pulse.isHigh});
                }
            } else if (module.type == '%') {
                if (!pulse.isHigh) {
                    module.isOn = !module.isOn;
                    for (const std::string& output : module.outputs) {
                        q.push({pulse.to, output, module.isOn});
                    }
                }
            } else if (module.type == '&') {
                module.memory[pulse.from] = pulse.isHigh;
                bool allHigh = std::all_of(module.memory.begin(), module.memory.end(),
                                          [](const auto& pair){ return pair.second; });
                for (const std::string& output : module.outputs) {
                    q.push({pulse.to, output, !allHigh});
                }
            }
        }
    }

    // Calculate the LCM of the cycle lengths
    long long result = 1;
    for (const auto& [name, length] : cycle_lengths) {
        result = lcm(result, length);
    }

    std::cout << result << std::endl;

    return 0;
}
