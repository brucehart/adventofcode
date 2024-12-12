#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

// Class to parse JSON and compute the required sum
class JSONParser {
public:
    JSONParser(const std::string& data) : json(data), index(0) {}

    // Initiates the parsing and returns the total sum
    long long parse() {
        return parseValue();
    }

private:
    const std::string& json;
    size_t index;

    // Parses any JSON value based on the current character
    long long parseValue() {
        if (index >= json.size()) {
            return 0;
        }

        char current = json[index];
        if (current == '{') {
            return parseObject();
        } else if (current == '[') {
            return parseArray();
        } else if (current == '"') {
            parseString(); // Strings are ignored unless they are "red" within objects
            return 0;
        } else if (current == '-' || std::isdigit(current)) {
            return parseNumber();
        } else {
            // For any other unexpected character, skip it
            ++index;
            return 0;
        }
    }

    // Parses a JSON object and returns the sum of its numbers
    long long parseObject() {
        ++index; // Skip '{'
        long long sum = 0;
        bool ignore = false;

        while (index < json.size() && json[index] != '}') {
            // Parse key
            std::string key = parseString();

            // Skip ':' delimiter
            if (json[index] == ':') {
                ++index;
            }

            // Check the value
            if (json[index] == '"') {
                std::string value = parseString();
                if (value == "red") {
                    ignore = true;
                }
            } else {
                long long valueSum = parseValue();
                sum += valueSum;
            }

            // Skip ',' delimiter if present
            if (json[index] == ',') {
                ++index;
            }
        }

        ++index; // Skip '}'

        // If the object contains "red", ignore its entire sum
        return ignore ? 0 : sum;
    }

    // Parses a JSON array and returns the sum of its numbers
    long long parseArray() {
        ++index; // Skip '['
        long long sum = 0;

        while (index < json.size() && json[index] != ']') {
            sum += parseValue();

            // Skip ',' delimiter if present
            if (json[index] == ',') {
                ++index;
            }
        }

        ++index; // Skip ']'
        return sum;
    }

    // Parses a JSON string and returns its content
    std::string parseString() {
        std::string result;
        ++index; // Skip initial '"'

        while (index < json.size() && json[index] != '"') {
            if (json[index] == '\\') {
                // Handle escaped characters
                ++index;
                if (index < json.size()) {
                    result += json[index];
                    ++index;
                }
            } else {
                result += json[index];
                ++index;
            }
        }

        ++index; // Skip closing '"'
        return result;
    }

    // Parses a JSON number and returns its integer value
    long long parseNumber() {
        size_t start = index;
        if (json[index] == '-') {
            ++index;
        }
        while (index < json.size() && std::isdigit(json[index])) {
            ++index;
        }

        // Convert the substring to a long long integer
        long long number = std::stoll(json.substr(start, index - start));
        return number;
    }
};

int main() {
    // Read entire input from stdin
    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
    }

    // Initialize parser and compute the sum
    JSONParser parser(input);
    long long totalSum = parser.parse();

    // Output the result
    std::cout << totalSum << std::endl;

    return 0;
}
