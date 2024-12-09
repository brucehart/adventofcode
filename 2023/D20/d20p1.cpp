#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

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

    long long lowPulses = 0;
    long long highPulses = 0;

    // Simulate button presses
    for (int i = 0; i < 1000; ++i) {
        lowPulses++; // Button press sends a low pulse
        std::queue<Pulse> q;
        q.push({"button", "broadcaster", false});

        while (!q.empty()) {
            Pulse pulse = q.front();
            q.pop();

            if (!modules.count(pulse.to)) continue;

            Module& module = modules[pulse.to];

            if (module.type == 'b') {
                for (const std::string& output : module.outputs) {
                    if (pulse.isHigh) highPulses++; else lowPulses++;
                    q.push({pulse.to, output, pulse.isHigh});
                }
            } else if (module.type == '%') {
                if (!pulse.isHigh) {
                    module.isOn = !module.isOn;
                    for (const std::string& output : module.outputs) {
                        if (module.isOn) highPulses++; else lowPulses++;
                        q.push({pulse.to, output, module.isOn});
                    }
                }
            } else if (module.type == '&') {
                module.memory[pulse.from] = pulse.isHigh;
                bool allHigh = std::all_of(module.memory.begin(), module.memory.end(),
                                          [](const auto& pair){ return pair.second; });
                for (const std::string& output : module.outputs) {
                    if (!allHigh) highPulses++; else lowPulses++;
                    q.push({pulse.to, output, !allHigh});
                }
            }
        }
    }

    std::cout << lowPulses * highPulses << std::endl;

    return 0;
}
