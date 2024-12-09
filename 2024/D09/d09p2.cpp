#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef unsigned long long ull;

// Parse input string into vectors of file and free space lengths
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
                    ull sum = (length * (2 * start + length - 1)) / 2;
                    checksum += sum * currentId;
                }
                // Start new segment
                start = i;
                currentId = finalPositions[i];
            }
        } else if (start != -1) {
            // Process segment ending at free space
            size_t length = i - start;
            ull sum = (length * (2 * start + length - 1)) / 2;
            checksum += sum * currentId;
            start = -1;
        }
    }
    
    // Handle last segment if exists
    if (start != -1) {
        size_t length = finalPositions.size() - start;
        ull sum = (length * (2 * start + length - 1)) / 2;
        checksum += sum * currentId;
    }
    
    return checksum;
}

std::vector<int> compactDiskWholeFiles(const std::vector<int>& files, const std::vector<int>& freeSpace) {
    std::vector<int> positions;
    int currentPos = 0;
    
    // Initialize disk layout
    for (int i = 0; i < files[0]; i++) {
        positions.push_back(0);
        currentPos++;
    }

    for (size_t i = 0; i < freeSpace.size(); i++) {
        for (int j = 0; j < freeSpace[i]; j++) {
            positions.push_back(-1);
            currentPos++;
        }
        
        if (i + 1 < files.size()) {
            for (int j = 0; j < files[i + 1]; j++) {
                positions.push_back(i + 1);
                currentPos++;
            }
        }
    }

    // Process files in reverse order (highest ID first)
    for (int fileId = files.size() - 1; fileId >= 0; fileId--) {
        int fileSize = files[fileId];
        int fileStart = -1;
        int fileEnd = -1;
        
        // Find current file position
        for (size_t i = 0; i < positions.size(); i++) {
            if (positions[i] == fileId) {
                if (fileStart == -1) fileStart = i;
                fileEnd = i;
            }
        }
        
        // Find leftmost viable free space
        int bestFreeStart = -1;
        int consecutiveFree = 0;
        
        for (size_t i = 0; i < fileStart; i++) {
            if (positions[i] == -1) {
                consecutiveFree++;
                if (consecutiveFree == fileSize && bestFreeStart == -1) {
                    bestFreeStart = i - fileSize + 1;
                }
            } else {
                consecutiveFree = 0;
            }
        }
        
        // Move file if viable space found
        if (bestFreeStart != -1) {
            // Clear old position
            for (int i = fileStart; i <= fileEnd; i++) {
                positions[i] = -1;
            }
            // Set new position
            for (int i = 0; i < fileSize; i++) {
                positions[bestFreeStart + i] = fileId;
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
    auto finalPositions = compactDiskWholeFiles(files, freeSpace);
    ull checksum = computeChecksum(finalPositions);
    
    std::cout << checksum << std::endl;
    return 0;
}