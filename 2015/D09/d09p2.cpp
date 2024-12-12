#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Maps each city to a unique index.
    std::map<std::string, int> cityIndex;
    std::vector<std::vector<int>> dist;
    int cityCount = 0;

    std::string cityA, cityB, to, eq;
    int d;
    while (std::cin >> cityA >> to >> cityB >> eq >> d) {
        // Check if cityA is a new city
        if (cityIndex.find(cityA) == cityIndex.end()) {
            cityIndex[cityA] = cityCount++;
            // Resize dist to be cityCount x cityCount, initializing with -1
            dist.resize(cityCount);
            for (auto &row : dist) {
                row.resize(cityCount, -1);
            }
        }

        // Check if cityB is a new city
        if (cityIndex.find(cityB) == cityIndex.end()) {
            cityIndex[cityB] = cityCount++;
            // Resize dist to be cityCount x cityCount, initializing with -1
            dist.resize(cityCount);
            for (auto &row : dist) {
                row.resize(cityCount, -1);
            }
        }

        int idxA = cityIndex[cityA];
        int idxB = cityIndex[cityB];

        dist[idxA][idxB] = d;
        dist[idxB][idxA] = d;
    }

    // If no cities, answer is trivial
    if (cityCount == 0) {
        std::cout << 0 << "\n";
        return 0;
    }

    // Create a vector of city indices to permute
    std::vector<int> cities(cityCount);
    for (int i = 0; i < cityCount; ++i) {
        cities[i] = i;
    }

    int maxDistance = 0;

    // Check all permutations of the cities
    do {
        int currentDist = 0;
        bool valid = true;
        for (int i = 0; i < cityCount - 1; ++i) {
            int from = cities[i];
            int toCity = cities[i + 1];
            if (dist[from][toCity] < 0) {
                valid = false;
                break;
            }
            currentDist += dist[from][toCity];
        }

        if (valid && currentDist > maxDistance) {
            maxDistance = currentDist;
        }

    } while (std::next_permutation(cities.begin(), cities.end()));

    std::cout << maxDistance << "\n";
    return 0;
}
