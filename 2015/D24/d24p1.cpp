#include <bits/stdc++.h>

// Function to check if any subset of 'nums' sums to 'target'
bool can_partition(const std::vector<int>& nums, int target) {
    // Initialize a vector for dynamic programming
    std::vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (const auto& num : nums) {
        // Iterate backwards to prevent using the same number multiple times
        for (int j = target; j >= num; --j) {
            if (dp[j - num]) {
                dp[j] = true;
            }
        }
    }

    return dp[target];
}

// Recursive function to generate combinations of size 'k' that sum to 'target_sum'
// Returns a vector of quantum entanglements for valid combinations
std::vector<unsigned long long> find_combinations(const std::vector<int>& packages, int target_sum, int k) {
    std::vector<unsigned long long> qes;
    std::vector<int> current;
    
    // Lambda function for backtracking
    std::function<void(int, int, unsigned long long)> backtrack = [&](int start, int remaining_sum, unsigned long long product) {
        if (current.size() == k) {
            if (remaining_sum == 0) {
                qes.push_back(product);
            }
            return;
        }
        for (int i = start; i < packages.size(); ++i) {
            // Prune if the current package exceeds the remaining sum
            if (packages[i] > remaining_sum) continue;
            // Avoid duplicates by skipping the same element
            if (i > start && packages[i] == packages[i - 1]) continue;
            current.push_back(packages[i]);
            backtrack(i + 1, remaining_sum - packages[i], product * packages[i]);
            current.pop_back();
        }
    };
    
    backtrack(0, target_sum, 1);
    return qes;
}

int main() {
    std::vector<int> packages;
    int weight;
    
    // Read input from stdin
    while (std::cin >> weight) {
        packages.push_back(weight);
    }

    // Calculate total sum and target sum per group
    long long total_sum = 0;
    for (const auto& w : packages) {
        total_sum += w;
    }

    if (total_sum % 3 != 0) {
        // It's impossible to split into three equal groups
        std::cout << "0\n";
        return 0;
    }

    int target_sum = static_cast<int>(total_sum / 3);

    // Sort packages in descending order to optimize combination generation
    std::sort(packages.begin(), packages.end(), std::greater<int>());

    int n = packages.size();
    unsigned long long minimal_qe = 0;
    bool found = false;

    // Iterate over possible group sizes starting from the smallest
    for (int group_size = 1; group_size <= n; ++group_size) {
        // Find all quantum entanglements for combinations of current group size
        std::vector<unsigned long long> qes = find_combinations(packages, target_sum, group_size);
        
        // Sort to process smaller quantum entanglements first
        std::sort(qes.begin(), qes.end());
        
        for (const auto& qe : qes) {
            // Reconstruct the combination from QE by trying to find the subset
            // Since QE is a product, and multiple subsets can have the same QE,
            // we need to find the actual subset. However, for simplicity and efficiency,
            // we'll iterate again to find the subset.

            // Find the combination that produces this QE
            std::vector<int> combination;
            unsigned long long current_qe = 1;
            int remaining_sum = target_sum;
            for (int i = 0; i < n; ++i) {
                if (current_qe * packages[i] > qe) continue;
                if (current_qe * packages[i] == qe && remaining_sum - packages[i] == 0 && combination.size() + 1 == group_size) {
                    combination.push_back(packages[i]);
                    break;
                }
                if (remaining_sum >= packages[i] && current_qe * packages[i] <= qe) {
                    combination.push_back(packages[i]);
                    current_qe *= packages[i];
                    remaining_sum -= packages[i];
                    if (combination.size() == group_size && remaining_sum == 0) {
                        break;
                    }
                }
            }

            // Create a mask for the selected combination
            std::vector<int> remaining_packages;
            std::vector<int> selected = combination;
            std::vector<int> temp_packages = packages;
            for (const auto& num : selected) {
                auto it = std::find(temp_packages.begin(), temp_packages.end(), num);
                if (it != temp_packages.end()) {
                    temp_packages.erase(it);
                }
            }
            remaining_packages = temp_packages;

            // Check if the remaining packages can be partitioned into two groups of target_sum
            if (can_partition(remaining_packages, target_sum)) {
                // Found the minimal QE
                minimal_qe = qe;
                found = true;
                break;
            }
        }

        if (found) {
            break;
        }
    }

    std::cout << minimal_qe << "\n";
    return 0;
}

