#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

typedef unsigned long long ull;

void parseInput(const std::string& data, std::vector<int>& files, std::vector<int>& freeSpace) {
    for (size_t i = 0; i < data.length(); i++) {
        int value = data[i] - '0';
        if (i % 2 == 0) {
            files.push_back(value);
        } else {
            freeSpace.push_back(value);
        }
    }
}

ull computeChecksum(const std::vector<int>& finalPositions) {
    ull checksum = 0;
    int start = -1;
    int currentId = -1;
    
    for (size_t i = 0; i < finalPositions.size(); i++) {
        if (finalPositions[i] >= 0) {
            if (currentId != finalPositions[i]) {
                // Process previous segment
                if (start != -1) {
                    size_t length = i - start;
                    ull sum = (length * (2ULL * start + length - 1)) / 2ULL;
                    checksum += sum * currentId;
                }
                // Start new segment
                start = (int)i;
                currentId = finalPositions[i];
            }
        } else if (start != -1) {
            // Process segment ending at free space
            size_t length = i - start;
            ull sum = (length * (2ULL * start + length - 1)) / 2ULL;
            checksum += sum * currentId;
            start = -1;
        }
    }
    
    // Handle last segment if exists
    if (start != -1) {
        size_t length = finalPositions.size() - start;
        ull sum = (length * (2ULL * start + length - 1)) / 2ULL;
        checksum += sum * currentId;
    }
    
    return checksum;
}

std::vector<int> compactDisk(const std::vector<int>& files, const std::vector<int>& freeSpace) {
    std::vector<int> positions;
    positions.reserve(std::accumulate(files.begin(), files.end(), 0) + 
                      std::accumulate(freeSpace.begin(), freeSpace.end(), 0));

    // Initialize disk layout
    // Place first file
    for (int i = 0; i < files[0]; i++) {
        positions.push_back(0);
    }

    // Interleave free spaces and subsequent files
    for (size_t i = 0; i < freeSpace.size(); i++) {
        for (int j = 0; j < freeSpace[i]; j++) {
            positions.push_back(-1);
        }
        if (i + 1 < files.size()) {
            for (int j = 0; j < files[i + 1]; j++) {
                positions.push_back((int)(i + 1));
            }
        }
    }

    // Precompute all free positions
    std::vector<int> freePositions;
    freePositions.reserve(positions.size());
    for (int i = 0; i < (int)positions.size(); i++) {
        if (positions[i] == -1) {
            freePositions.push_back(i);
        }
    }
    // freePositions is now sorted in ascending order

    // We'll use an index to track the next available free position
    size_t freeIdx = 0;

    // Compact disk by moving files to the earliest possible free position
    // Iterate from right to left
    for (int pos = (int)positions.size() - 1; pos >= 0; pos--) {
        if (positions[pos] >= 0) {
            // If we have run out of free positions, we can't move this block
            if (freeIdx >= freePositions.size()) continue;

            // We need a free position strictly less than 'pos' to move this file block
            // Advance freeIdx until we find such a free position or run out
            while (freeIdx < freePositions.size() && freePositions[freeIdx] >= pos) {
                freeIdx++;
            }

            if (freeIdx < freePositions.size() && freePositions[freeIdx] < pos) {
                int fileBlock = positions[pos];
                int target = freePositions[freeIdx];
                // Move file block to free space
                positions[target] = fileBlock;
                positions[pos] = -1;
                // This position is now free, but we won't reuse it because we're going right to left
                freeIdx++; // use next free position next time
            }
        }
    }

    return positions;
}

int main() {
    std::string data;
    std::getline(std::cin, data);

    std::vector<int> files;
    std::vector<int> freeSpace;
    
    parseInput(data, files, freeSpace);
    auto finalPositions = compactDisk(files, freeSpace);
    ull checksum = computeChecksum(finalPositions);
    
    std::cout << checksum << std::endl;
    return 0;
}
