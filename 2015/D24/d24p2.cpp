#include <bits/stdc++.h>

// Function to check if the packages can be partitioned into 'k' groups each summing to 'target'
// This uses recursive backtracking to assign packages to each group
bool can_partition_k(const std::vector<int>& packages, int target, int k, 
                     std::vector<int>& groups, int index) {
    if (k == 1) {
        // Only one group left, no need to check as the remaining packages must sum to target
        return true;
    }

    if (groups[k - 1] == target) {
        // Current group is filled, move to the next group
        return can_partition_k(packages, target, k - 1, groups, 0);
    }

    for (int i = index; i < packages.size(); ++i) {
        if (groups[k - 1] + packages[i] > target) {
            continue; // Skip if adding this package exceeds the target
        }

        // Skip duplicates to avoid redundant work
        if (i > index && packages[i] == packages[i - 1]) {
            continue;
        }

        groups[k - 1] += packages[i];

        if (can_partition_k(packages, target, k, groups, i + 1)) {
            return true;
        }

        groups[k - 1] -= packages[i];
    }

    return false;
}

// Function to check if the packages can be partitioned into 'k' groups each summing to 'target_sum'
bool can_partition(const std::vector<int>& packages, int target_sum, int k) {
    std::vector<int> sorted_packages = packages;
    // Sort in descending order to optimize the backtracking
    std::sort(sorted_packages.begin(), sorted_packages.end(), std::greater<int>());

    std::vector<int> groups(k, 0);
    return can_partition_k(sorted_packages, target_sum, k, groups, 0);
}

// Recursive function to generate combinations of size 'k' that sum to 'target_sum'
// Returns a vector of combinations, where each combination is a vector of package weights
void find_combinations(const std::vector<int>& packages, int target_sum, int k, 
                      int start, std::vector<int>& current, std::vector<std::vector<int>>& results) {
    if (current.size() == k) {
        if (target_sum == 0) {
            results.emplace_back(current);
        }
        return;
    }

    for (int i = start; i < packages.size(); ++i) {
        if (packages[i] > target_sum) {
            continue; // Skip if the current package exceeds the remaining sum
        }

        // Skip duplicates to avoid redundant combinations
        if (i > start && packages[i] == packages[i - 1]) {
            continue;
        }

        current.push_back(packages[i]);
        find_combinations(packages, target_sum - packages[i], k, i + 1, current, results);
        current.pop_back();
    }
}

int main() {
    std::vector<int> packages;
    int weight;

    // Read input from stdin
    while (std::cin >> weight) {
        packages.push_back(weight);
    }

    // Calculate total sum and verify it's divisible by 4
    long long total_sum = 0;
    for (const auto& w : packages) {
        total_sum += w;
    }

    if (total_sum % 4 != 0) {
        // It's impossible to split into four equal groups
        std::cout << "0\n";
        return 0;
    }

    int target_sum = static_cast<int>(total_sum / 4);

    // Sort packages in descending order to optimize combination generation
    std::sort(packages.begin(), packages.end(), std::greater<int>());

    int n = packages.size();
    unsigned long long minimal_qe = 0;
    bool found = false;

    // Iterate over possible group sizes starting from the smallest
    for (int group_size = 1; group_size <= n; ++group_size) {
        std::vector<std::vector<int>> valid_combinations;
        std::vector<int> current_combination;

        // Find all combinations of the current group size that sum to target_sum
        find_combinations(packages, target_sum, group_size, 0, current_combination, valid_combinations);

        // If no valid combinations found for this group size, continue to the next size
        if (valid_combinations.empty()) {
            continue;
        }

        // Sort the valid combinations based on their quantum entanglement (ascending)
        std::sort(valid_combinations.begin(), valid_combinations.end(),
                  [&](const std::vector<int>& a, const std::vector<int>& b) -> bool {
                      unsigned long long qe_a = 1, qe_b = 1;
                      for (const auto& num : a) qe_a *= num;
                      for (const auto& num : b) qe_b *= num;
                      return qe_a < qe_b;
                  });

        // Iterate through the sorted combinations to find the one with minimal QE
        for (const auto& combination : valid_combinations) {
            // Compute quantum entanglement for the current combination
            unsigned long long qe = 1;
            for (const auto& num : combination) {
                qe *= num;
            }

            // Create a list of remaining packages after removing the current combination
            std::vector<int> remaining_packages = packages;
            for (const auto& num : combination) {
                // Remove one instance of 'num' from remaining_packages
                auto it = std::find(remaining_packages.begin(), remaining_packages.end(), num);
                if (it != remaining_packages.end()) {
                    remaining_packages.erase(it);
                }
            }

            // Check if the remaining packages can be partitioned into 3 groups of target_sum
            if (can_partition(remaining_packages, target_sum, 3)) {
                // Found the minimal QE for the smallest group size
                minimal_qe = qe;
                found = true;
                break;
            }
        }

        if (found) {
            break; // No need to check larger group sizes
        }
    }

    std::cout << minimal_qe << "\n";
    return 0;
}

