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

// Helper function to trim leading and trailing whitespaces
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

// Function to determine if a monkey's value depends on 'humn'
bool depends_on_humn(const std::string& name,
                     const std::unordered_map<std::string, MonkeyJob>& jobs,
                     std::unordered_map<std::string, bool>& memo) {
    if (name == "humn") return true;
    if (memo.find(name) != memo.end()) return memo[name];
    auto it = jobs.find(name);
    if (it == jobs.end()) {
        throw std::runtime_error("Undefined monkey: " + name);
    }
    const MonkeyJob& job = it->second;
    if (job.is_number) {
        memo[name] = false;
        return false;
    }
    bool left_dep = depends_on_humn(job.left, jobs, memo);
    bool right_dep = depends_on_humn(job.right, jobs, memo);
    memo[name] = left_dep || right_dep;
    return memo[name];
}

// Function to compute the value yelled by a monkey
long long compute_value(const std::string& name,
                        const std::unordered_map<std::string, MonkeyJob>& jobs,
                        long long humn_value,
                        std::unordered_map<std::string, long long>& cache) {
    if (name == "humn") {
        return humn_value;
    }

    auto cache_it = cache.find(name);
    if (cache_it != cache.end()) {
        return cache_it->second;
    }

    auto it = jobs.find(name);
    if (it == jobs.end()) {
        throw std::runtime_error("Undefined monkey: " + name);
    }

    const MonkeyJob& job = it->second;

    if (job.is_number) {
        cache[name] = job.number;
        return job.number;
    }

    long long left_val = compute_value(job.left, jobs, humn_value, cache);
    long long right_val = compute_value(job.right, jobs, humn_value, cache);
    long long result = 0;

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

    cache[name] = result;
    return result;
}

// Function to solve for 'humn' by traversing the dependent branch
long long solve_for_humn(const std::string& name,
                          const std::unordered_map<std::string, MonkeyJob>& jobs,
                          long long target,
                          std::unordered_map<std::string, bool>& depends_memo) {
    if (name == "humn") {
        return target;
    }

    auto it = jobs.find(name);
    if (it == jobs.end()) {
        throw std::runtime_error("Undefined monkey: " + name);
    }

    const MonkeyJob& job = it->second;

    // If it's a number, it should match the target
    if (job.is_number) {
        if (job.number != target) {
            throw std::runtime_error("Mismatch in target at monkey: " + name);
        }
        return target;
    }

    // Determine which side depends on 'humn'
    bool left_dep = depends_on_humn(job.left, jobs, depends_memo);
    bool right_dep = depends_on_humn(job.right, jobs, depends_memo);

    if (left_dep && right_dep) {
        throw std::runtime_error("Both sides depend on 'humn' for monkey: " + name);
    }
    if (!left_dep && !right_dep) {
        throw std::runtime_error("Neither side depends on 'humn' for monkey: " + name);
    }

    // Compute the value of the side that does not depend on 'humn'
    long long known_val = 0;
    std::string dependent_side;
    std::string other_side = "";
    char op = job.op;

    if (left_dep) {
        dependent_side = job.left;
        // Compute right_val
        std::unordered_map<std::string, long long> cache;
        known_val = compute_value(job.right, jobs, 0, cache); // humn is not involved here
    }
    else { // right_dep
        dependent_side = job.right;
        // Compute left_val
        std::unordered_map<std::string, long long> cache;
        known_val = compute_value(job.left, jobs, 0, cache);
    }

    // Invert the operation to find the new target
    MonkeyJob dependent_job = jobs.at(dependent_side);
    switch (job.op) {
        case '+':
            // If dependent_side + known_val = target, then dependent_side = target - known_val
            target = target - known_val;
            break;
        case '-':
            if (left_dep) {
                // left - right = target => left = target + right_val
                target = target + known_val;
            }
            else {
                // left - right = target => right = left_val - target
                target = known_val - target;
            }
            break;
        case '*':
            if (known_val == 0) {
                throw std::runtime_error("Multiplication by zero encountered.");
            }
            // dependent_side * known_val = target => dependent_side = target / known_val
            if (target % known_val != 0) {
                throw std::runtime_error("Non-integer division encountered.");
            }
            target = target / known_val;
            break;
        case '/':
            if (left_dep) {
                // left / right = target => left = target * right_val
                target = target * known_val;
            }
            else {
                // left / right = target => right = left_val / target
                if (target == 0) {
                    throw std::runtime_error("Division by zero encountered while solving.");
                }
                target = known_val / target;
            }
            break;
        default:
            throw std::runtime_error("Unknown operator while solving for 'humn': " + std::string(1, job.op));
    }

    // Recursively solve for the dependent side
    return solve_for_humn(dependent_side, jobs, target, depends_memo);
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

        if (name == "root") {
            // For Part Two, root's operation is '='
            if (tokens.size() != 3) {
                throw std::runtime_error("Invalid root job format.");
            }
            // Store root's left and right monkeys without setting an operator
            job.is_number = false;
            job.left = tokens[0];
            job.op = '='; // Special operator for equality
            job.right = tokens[2];
        }
        else {
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
            }
            else if (tokens.size() == 3) {
                // Job is an operation: operand1 operator operand2
                job.is_number = false;
                job.left = tokens[0];
                if (tokens[1].size() != 1 || std::string("+-*/").find(tokens[1][0]) == std::string::npos) {
                    throw std::runtime_error("Invalid operator for monkey: " + name);
                }
                job.op = tokens[1][0];
                job.right = tokens[2];
            }
            else {
                throw std::runtime_error("Invalid job format for monkey: " + name);
            }
        }

        // Store the job in the map
        jobs[name] = job;
    }

    // Identify the two monkeys that root listens to
    if (jobs.find("root") == jobs.end()) {
        std::cerr << "Error: 'root' monkey not found in the input." << std::endl;
        return 1;
    }

    const MonkeyJob& root_job = jobs["root"];
    std::string root_left = root_job.left;
    std::string root_right = root_job.right;

    // Determine which side depends on 'humn'
    std::unordered_map<std::string, bool> depends_memo;
    bool left_dep = false, right_dep = false;
    try {
        left_dep = (root_left != "humn") ? false : true; // Initialize to false; will compute below
        right_dep = (root_right != "humn") ? false : true; // Initialize to false; will compute below

        left_dep = depends_on_humn(root_left, jobs, depends_memo);
        right_dep = depends_on_humn(root_right, jobs, depends_memo);
    }
    catch (const std::exception& e) {
        std::cerr << "Error while determining dependencies: " << e.what() << std::endl;
        return 1;
    }

    if (left_dep && right_dep) {
        std::cerr << "Error: Both branches of root depend on 'humn'. Cannot solve uniquely." << std::endl;
        return 1;
    }
    if (!left_dep && !right_dep) {
        std::cerr << "Error: Neither branch of root depends on 'humn'. No solution needed." << std::endl;
        return 1;
    }

    // Compute the value of the branch that does not depend on 'humn'
    long long known_val = 0;
    std::string dependent_side;
    if (left_dep) {
        dependent_side = root_left;
        // Compute the value of the right branch
        std::unordered_map<std::string, long long> cache;
        try {
            known_val = compute_value(root_right, jobs, 0, cache);
        }
        catch (const std::exception& e) {
            std::cerr << "Error while computing known_val: " << e.what() << std::endl;
            return 1;
        }
    }
    else { // right_dep
        dependent_side = root_right;
        // Compute the value of the left branch
        std::unordered_map<std::string, long long> cache;
        try {
            known_val = compute_value(root_left, jobs, 0, cache);
        }
        catch (const std::exception& e) {
            std::cerr << "Error while computing known_val: " << e.what() << std::endl;
            return 1;
        }
    }

    // Now, traverse the dependent branch to solve for 'humn'
    long long required_humn = 0;
    try {
        required_humn = solve_for_humn(dependent_side, jobs, known_val, depends_memo);
    }
    catch (const std::exception& e) {
        std::cerr << "Error while solving for 'humn': " << e.what() << std::endl;
        return 1;
    }

    // Output the required humn value
    std::cout << required_humn << std::endl;

    return 0;
}

