#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <functional>
#include <algorithm>
#include <sstream>
#include <cctype>

// Structure to represent each Monkey
struct Monkey {
    std::deque<long> items;                       // Queue of worry levels
    std::function<long(long)> operation;          // Operation to modify worry level
    int test_divisor;                             // Divisor for the test
    int if_true;                                  // Target monkey if test is true
    int if_false;                                 // Target monkey if test is false
    long inspect_count = 0;                       // Number of inspections
};

// Helper function to trim leading and trailing whitespace
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

int main() {
    std::vector<Monkey> monkeys; // List of all monkeys
    std::string line;

    // Temporary variables to hold monkey attributes during parsing
    Monkey current_monkey;
    bool parsing_monkey = false;

    while (std::getline(std::cin, line)) {
        line = trim(line);
        if (line.empty()) {
            continue; // Skip empty lines
        }

        if (line.find("Monkey") == 0) {
            if (parsing_monkey) {
                monkeys.push_back(current_monkey); // Save the previous monkey
                current_monkey = Monkey();         // Reset for the next monkey
            } else {
                parsing_monkey = true;
            }
            continue; // Proceed to the next line
        }

        // Parse Starting items
        if (line.find("Starting items:") == 0) {
            size_t pos = line.find(":");
            std::string items_str = line.substr(pos + 1);
            std::stringstream ss(items_str);
            std::string item;
            while (std::getline(ss, item, ',')) {
                current_monkey.items.push_back(std::stol(trim(item)));
            }
            continue;
        }

        // Parse Operation
        if (line.find("Operation:") == 0) {
            size_t eq_pos = line.find('=');
            std::string expr = trim(line.substr(eq_pos + 1)); // e.g., "old * 19"
            std::stringstream ss(expr);
            std::string operand1, op, operand2;
            ss >> operand1 >> op >> operand2;

            // Define the operation based on the operator and operand
            if (op == "+") {
                if (operand2 == "old") {
                    current_monkey.operation = [](long old) -> long {
                        return old + old;
                    };
                } else {
                    long value = std::stol(operand2);
                    current_monkey.operation = [value](long old) -> long {
                        return old + value;
                    };
                }
            } else if (op == "*") {
                if (operand2 == "old") {
                    current_monkey.operation = [](long old) -> long {
                        return old * old;
                    };
                } else {
                    long value = std::stol(operand2);
                    current_monkey.operation = [value](long old) -> long {
                        return old * value;
                    };
                }
            }
            continue;
        }

        // Parse Test divisor
        if (line.find("Test: divisible by") == 0) {
            size_t pos = line.find_last_of(' ');
            current_monkey.test_divisor = std::stoi(line.substr(pos + 1));
            continue;
        }

        // Parse If true
        if (line.find("If true: throw to monkey") == 0) {
            size_t pos = line.find_last_of(' ');
            current_monkey.if_true = std::stoi(line.substr(pos + 1));
            continue;
        }

        // Parse If false
        if (line.find("If false: throw to monkey") == 0) {
            size_t pos = line.find_last_of(' ');
            current_monkey.if_false = std::stoi(line.substr(pos + 1));
            continue;
        }
    }

    // After reading all lines, don't forget to add the last monkey
    if (parsing_monkey) {
        monkeys.push_back(current_monkey);
    }

    const int TOTAL_ROUNDS = 20;

    // Simulate the rounds
    for (int round = 0; round < TOTAL_ROUNDS; ++round) {
        for (size_t i = 0; i < monkeys.size(); ++i) {
            Monkey& monkey = monkeys[i];
            // Process all items currently held by the monkey
            while (!monkey.items.empty()) {
                long item = monkey.items.front();
                monkey.items.pop_front();
                monkey.inspect_count++; // Increment inspection count

                // Apply the operation
                long new_worry = monkey.operation(item);

                // Apply relief by dividing by 3 and rounding down
                new_worry /= 3;

                // Determine the target monkey based on the test
                int target = (new_worry % monkey.test_divisor == 0) ? monkey.if_true : monkey.if_false;

                // Throw the item to the target monkey
                monkeys[target].items.push_back(new_worry);
            }
        }
    }

    // Collect all inspection counts
    std::vector<long> inspection_counts;
    for (const auto& monkey : monkeys) {
        inspection_counts.push_back(monkey.inspect_count);
    }

    // Find the two highest inspection counts
    std::sort(inspection_counts.begin(), inspection_counts.end(), std::greater<long>());
    long most_active = inspection_counts[0];
    long second_most_active = inspection_counts[1];

    // Calculate the level of monkey business
    long monkey_business_level = most_active * second_most_active;

    // Output the result
    std::cout << monkey_business_level << std::endl;

    return 0;
}

