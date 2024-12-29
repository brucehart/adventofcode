#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

// Structure to represent each monkey's job
struct MonkeyJob {
    bool is_number;          // Indicates if the job is a direct number
    long long number;        // The number value if is_number is true
    char op;                 // The operator if the job is an operation
    std::string left;        // Left operand's monkey name
    std::string right;       // Right operand's monkey name

    MonkeyJob() : is_number(false), number(0), op(0), left(""), right("") {}
};

// Function to compute the value yelled by a monkey
long long compute_value(
    const std::string& name,
    const std::unordered_map<std::string, MonkeyJob>& jobs,
    std::unordered_map<std::string, long long>& cache
) {
    // If the value is already computed, return it from the cache
    auto cache_it = cache.find(name);
    if (cache_it != cache.end()) {
        return cache_it->second;
    }

    // Retrieve the monkey's job
    auto job_it = jobs.find(name);
    if (job_it == jobs.end()) {
        throw std::runtime_error("Undefined monkey: " + name);
    }
    const MonkeyJob& job = job_it->second;

    // If the job is a number, store and return it
    if (job.is_number) {
        cache[name] = job.number;
        return job.number;
    }

    // Recursively compute the values of the left and right operands
    long long left_val = compute_value(job.left, jobs, cache);
    long long right_val = compute_value(job.right, jobs, cache);
    long long result = 0;

    // Perform the operation based on the operator
    switch (job.op) {
        case '+':
            result = left_val + right_val;
            break;
        case '-':
            result = left_val - right_val;
            break;
        case '*':
            result = left_val * right_val;
            break;
        case '/':
            if (right_val == 0) {
                throw std::runtime_error("Division by zero in monkey: " + name);
            }
            result = left_val / right_val;
            break;
        default:
            throw std::runtime_error("Unknown operator in monkey: " + name);
    }

    // Store the computed result in the cache and return it
    cache[name] = result;
    return result;
}

// Helper function to trim leading and trailing whitespaces
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

int main() {
    std::unordered_map<std::string, MonkeyJob> jobs; // Map to store each monkey's job
    std::string line;

    // Read input line by line
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue; // Skip empty lines

        // Find the position of the first colon
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) {
            throw std::runtime_error("Invalid line format (missing colon): " + line);
        }

        // Extract the monkey name and job description
        std::string name = trim(line.substr(0, colon_pos));
        std::string job_desc = trim(line.substr(colon_pos + 1));

        MonkeyJob job;

        // Split the job description into tokens
        std::istringstream job_stream(job_desc);
        std::vector<std::string> tokens;
        std::string token;
        while (job_stream >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            throw std::runtime_error("Invalid job description for monkey: " + name);
        }

        if (tokens.size() == 1) {
            // Job is a direct number
            try {
                job.is_number = true;
                job.number = std::stoll(tokens[0]);
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid number for monkey: " + name);
            } catch (const std::out_of_range&) {
                throw std::runtime_error("Number out of range for monkey: " + name);
            }
        } else if (tokens.size() == 3) {
            // Job is an operation: operand1 operator operand2
            job.is_number = false;
            job.left = tokens[0];
            if (tokens[1].size() != 1 || std::string("+-*/").find(tokens[1][0]) == std::string::npos) {
                throw std::runtime_error("Invalid operator for monkey: " + name);
            }
            job.op = tokens[1][0];
            job.right = tokens[2];
        } else {
            throw std::runtime_error("Invalid job format for monkey: " + name);
        }

        // Store the job in the map
        jobs[name] = job;
    }

    // Cache to store computed values for memoization
    std::unordered_map<std::string, long long> cache;

    try {
        // Compute the value yelled by the 'root' monkey
        long long root_value = compute_value("root", jobs, cache);
        std::cout << root_value << std::endl;
    } catch (const std::exception& e) {
        // Handle any errors that occur during computation
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
