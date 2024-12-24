#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Structure representing a logic gate with its operation and input wires
struct LogicGate {
    std::string operation;
    std::pair<std::string, std::string> inputs;
};

// Type aliases for clarity
using WireStates = std::unordered_map<std::string, bool>;
using GateMap = std::unordered_map<std::string, LogicGate>;
using OutputList = std::vector<std::string>;

// Function to retrieve the output wire based on inputs and operation
std::string findGateOutput(const std::string& input1, const std::string& input2,
                           const std::string& operation, const GateMap& gates) {
    for (const auto& [output, gate] : gates) {
        if (gate.operation != operation)
            continue;
        const auto& [gateInput1, gateInput2] = gate.inputs;
        if ((input1 == gateInput1 && input2 == gateInput2) ||
            (input1 == gateInput2 && input2 == gateInput1)) {
            return output;
        }
    }
    return "";
}

// Function to swap two gates and update all related outputs accordingly
void swapLogicGates(const std::string& gateA, const std::string& gateB, GateMap& gates,
                   OutputList& outputsA, OutputList& outputsB,
                   OutputList& outputsC, OutputList& outputsD) {
    std::swap(gates[gateA], gates[gateB]);

    auto updateOutputs = [&](OutputList& outputs) {
        for (auto& wire : outputs) {
            if (wire == gateA)
                wire = gateB;
            else if (wire == gateB)
                wire = gateA;
        }
    };

    updateOutputs(outputsA);
    updateOutputs(outputsB);
    updateOutputs(outputsC);
    updateOutputs(outputsD);
}

// Function to parse gate definitions from the input stream
void parseGateDefinitions(std::istream& input, GateMap& gates,
                          OutputList& xorOutputs, OutputList& andOutputs) {
    std::string input1, operation, input2, arrow, output;
    while (input >> input1 >> operation >> input2 >> arrow >> output) {
        gates[output] = LogicGate{operation, {input1, input2}};

        // Populate XOR and AND output lists based on specific input patterns
        if ((input1.rfind("x", 0) == 0 && input2.rfind("y", 0) == 0) ||
            (input1.rfind("y", 0) == 0 && input2.rfind("x", 0) == 0)) {
            int index1 = std::stoi(input1.substr(1));
            int index2 = std::stoi(input2.substr(1));

            if (index1 != index2)
                continue;

            if (static_cast<size_t>(index1 + 1) > xorOutputs.size()) {
                xorOutputs.resize(index1 + 1, "");
                andOutputs.resize(index1 + 1, "");
            }

            if (operation == "XOR") {
                xorOutputs[index1] = output;
            } else if (operation == "AND") {
                andOutputs[index1] = output;
            }
        }
    }
}

// Function to identify expected incorrect outputs based on gate configurations
OutputList identifyIncorrectOutputs(const GateMap& gates, const OutputList& xorOutputs,
                                    const OutputList& andOutputs, OutputList& intermediateC,
                                    OutputList& intermediateD) {
    OutputList incorrectGates;

    // Validate the first gate "z00"
    auto firstGateIt = gates.find("z00");
    if (firstGateIt != gates.end()) {
        const LogicGate& firstGate = firstGateIt->second;
        const auto& [in1, in2] = firstGate.inputs;
        bool invalidInputs = !((in1 == "x00" && in2 == "y00") ||
                               (in1 == "y00" && in2 == "x00"));
        if (firstGate.operation != "XOR" || invalidInputs)
            incorrectGates.emplace_back("z00");
    } else {
        incorrectGates.emplace_back("z00");
    }

    intermediateC.resize(xorOutputs.size(), "");
    intermediateD.resize(xorOutputs.size(), "");
    if (!andOutputs.empty())
        intermediateC[1] = andOutputs[0];

    // Iterate through outputs from "z01" to "z99"
    for (size_t i = 1; i < xorOutputs.size(); ++i) {
        std::ostringstream wireLabel;
        wireLabel.width(2);
        wireLabel.fill('0');
        wireLabel << i;
        std::string currentWire = "z" + wireLabel.str();

        auto gateIt = gates.find(currentWire);
        if (gateIt == gates.end()) {
            incorrectGates.emplace_back(currentWire);
            continue;
        }

        const LogicGate& currentGate = gateIt->second;
        const auto& [inputA, inputB] = currentGate.inputs;

        if (currentGate.operation != "XOR") {
            incorrectGates.emplace_back(currentWire);

            // Update intermediate outputs based on AND and OR operations
            intermediateD[i] = findGateOutput(xorOutputs[i - 1], intermediateC[i - 1], "AND", gates);
            intermediateC[i] = findGateOutput(andOutputs[i - 1], intermediateD[i], "OR", gates);

            // Attempt to find a gate to swap
            std::string swapGate = findGateOutput(xorOutputs[i], intermediateC[i], "XOR", gates);
            if (!swapGate.empty()) {
                swapLogicGates(swapGate, currentWire, const_cast<GateMap&>(gates),
                              const_cast<OutputList&>(xorOutputs),
                              const_cast<OutputList&>(andOutputs),
                              intermediateC, intermediateD);
                incorrectGates.emplace_back(swapGate);
            }
        } else if (inputA != xorOutputs[i] && inputB != xorOutputs[i]) {
            incorrectGates.emplace_back(xorOutputs[i]);

            // Determine which gate to swap based on connections
            auto gateAIt = gates.find(inputA);
            auto gateBIt = gates.find(inputB);
            if (gateAIt != gates.end() &&
                (gateAIt->second.inputs.first == andOutputs[i - 1] ||
                 gateAIt->second.inputs.second == andOutputs[i - 1])) {
                swapLogicGates(xorOutputs[i], inputB, const_cast<GateMap&>(gates),
                              const_cast<OutputList&>(xorOutputs),
                              const_cast<OutputList&>(andOutputs),
                              intermediateC, intermediateD);
            } else if (gateBIt != gates.end() &&
                       (gateBIt->second.inputs.first == andOutputs[i - 1] ||
                        gateBIt->second.inputs.second == andOutputs[i - 1])) {
                swapLogicGates(xorOutputs[i], inputA, const_cast<GateMap&>(gates),
                              const_cast<OutputList&>(xorOutputs),
                              const_cast<OutputList&>(andOutputs),
                              intermediateC, intermediateD);
            }

            incorrectGates.emplace_back(xorOutputs[i]);
        }

        if (i > 1) {
            intermediateD[i] = findGateOutput(xorOutputs[i - 1], intermediateC[i - 1], "AND", gates);
            intermediateC[i] = findGateOutput(andOutputs[i - 1], intermediateD[i], "OR", gates);
        }
    }

    return incorrectGates;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    WireStates wireStates;
    GateMap gateDefinitions;
    OutputList xorOutputs;
    OutputList andOutputs;
    OutputList intermediateC;
    OutputList intermediateD;
    OutputList incorrectGates;

    std::string inputLine;

    // Read initial wire states until an empty line is encountered
    while (std::getline(std::cin, inputLine) && !inputLine.empty()) {
        size_t delimiterPos = inputLine.find(':');
        if (delimiterPos == std::string::npos)
            continue;
        std::string wireName = inputLine.substr(0, delimiterPos);
        int wireValue = std::stoi(inputLine.substr(delimiterPos + 1));
        wireStates[wireName] = static_cast<bool>(wireValue);
    }

    // Parse gate definitions from the remaining input
    parseGateDefinitions(std::cin, gateDefinitions, xorOutputs, andOutputs);

    // Identify incorrect gates based on the gate configurations
    incorrectGates = identifyIncorrectOutputs(gateDefinitions, xorOutputs, andOutputs,
                                             intermediateC, intermediateD);

    // Sort the list of incorrect gates lexicographically
    std::sort(incorrectGates.begin(), incorrectGates.end());

    // Prepare the output string with comma-separated incorrect gates
    std::string output;
    for (size_t i = 0; i < incorrectGates.size(); ++i) {
        output += incorrectGates[i];
        if (i != incorrectGates.size() - 1)
            output += ",";
    }

    // Output the result
    std::cout << output << std::endl;

    return 0;
}
