#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // Read container capacities from stdin
    std::vector<int> containers;
    int capacity;
    while (std::cin >> capacity) {
        containers.push_back(capacity);
    }

    int target_volume = 150;
    int combination_count = 0;
    int num_containers = containers.size();

    // Iterate through all possible combinations of containers using bit manipulation.
    for (int i = 0; i < (1 << num_containers); ++i) {
        int current_volume = 0;
        // Iterate through each container to check if it's included in this combination
        for (int j = 0; j < num_containers; ++j) {
            // If the jth bit is set, then include the jth container
            if ((i >> j) & 1) {
                current_volume += containers[j];
            }
        }

        // Check if the current combination sums to the target volume.
        if (current_volume == target_volume) {
            combination_count++;
        }
    }

    // Output the total number of combinations
    std::cout << combination_count << std::endl;

    return 0;
}
