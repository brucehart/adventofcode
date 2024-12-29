#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

// The decryption key specified for part 2.
constexpr long long DECRYPTION_KEY = 811589153;

// Number of times to apply the mixing operation for part 2.
constexpr int MIX_COUNT = 10;

// Steps after '0' to find the grove coordinates.
static const std::vector<int> STEPS = {1000, 2000, 3000};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read numbers from standard input into a vector until EOF
    std::vector<long long> numbers;
    long long value;
    while (std::cin >> value) {
        numbers.push_back(value);
    }

    // Multiply every number by the decryption key (part 2 requirement)
    std::transform(numbers.begin(), numbers.end(), numbers.begin(),
                   [](long long n) { return n * DECRYPTION_KEY; });

    // Create a list from the vector for efficient insertion/splicing
    std::list<long long> lst(numbers.begin(), numbers.end());

    // Create a vector of iterators to the list elements
    // so we can maintain the original order during mixing
    std::vector<std::list<long long>::iterator> iterators;
    iterators.reserve(numbers.size());
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        iterators.push_back(it);
    }

    // Perform the mixing process MIX_COUNT times
    for (int mixIteration = 0; mixIteration < MIX_COUNT; ++mixIteration) {
        for (auto it : iterators) {
            long long val = *it;

            // Skip moving if the value is zero
            if (val == 0) {
                continue;
            }

            // Calculate shift using modulo to handle large moves
            // We use (list_size - 1) because moving an element out of its spot 
            // effectively reduces the cycle by 1 during the splice
            auto listSize = static_cast<long long>(lst.size());
            long long shift = val % (listSize - 1);

            // Adjust negative shift to be positive
            if (shift < 0) {
                shift += (listSize - 1);
            }

            // If shift is zero after adjustment, no movement is needed
            if (shift == 0) {
                continue;
            }

            // Advance 'pos' by 'shift' positions
            auto pos = it;
            for (long long i = 0; i < shift; ++i) {
                ++pos;
                if (pos == lst.end()) {
                    pos = lst.begin();  // wrap around
                }
            }

            // Insert the element after 'pos' by advancing one more time
            ++pos;
            if (pos == lst.end()) {
                pos = lst.begin();  // wrap around if necessary
            }

            // Move the element to the new position
            lst.splice(pos, lst, it);
        }
    }

    // Locate '0' in the list
    auto zeroIt = std::find(lst.begin(), lst.end(), 0);

    // If there's no zero, output 0 and return
    if (zeroIt == lst.end()) {
        std::cout << 0 << std::endl;
        return 0;
    }

    // Sum the values at 1000th, 2000th, and 3000th positions after '0'
    long long sum = 0;
    for (int step : STEPS) {
        auto it = zeroIt;
        for (int i = 0; i < step; ++i) {
            ++it;
            if (it == lst.end()) {
                it = lst.begin();  // wrap around
            }
        }
        sum += *it;
    }

    // Output the result
    std::cout << sum << std::endl;
    return 0;
}
