#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

int main() {
    // Read container capacities from stdin
    std::vector<int> containers;
    int capacity;
    while (std::cin >> capacity) {
        containers.push_back(capacity);
    }

    int target_volume = 150;
    int num_containers = containers.size();
    int min_containers = std::numeric_limits<int>::max();
    int min_container_count = 0;

    // Iterate through all possible combinations of containers using bit manipulation.
    for (int i = 0; i < (1 << num_containers); ++i) {
        int current_volume = 0;
        int current_container_count = 0;
        // Iterate through each container to check if it's included in this combination
        for (int j = 0; j < num_containers; ++j) {
            // If the jth bit is set, then include the jth container
            if ((i >> j) & 1) {
                current_volume += containers[j];
                current_container_count++;
            }
        }

        // Check if the current combination sums to the target volume.
        if (current_volume == target_volume) {
            // If the container count is less than the current min, update min and reset count
            if(current_container_count < min_containers)
            {
              min_containers = current_container_count;
              min_container_count = 1;
            }
            // If the container count matches min, increment count
            else if (current_container_count == min_containers)
            {
              min_container_count++;
            }
        }
    }

    // Output the total number of combinations with min containers
    std::cout << min_container_count << std::endl;

    return 0;
}
