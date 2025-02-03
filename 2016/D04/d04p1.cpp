#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

// This program computes the sum of sector IDs for "real" rooms.
// Each room line is in the format:
//   encrypted-name-sectorID[checksum]
// The room is real if the checksum is equal to the five most common letters in the name,
// sorted primarily by descending frequency and then alphabetically.
int main() {
    std::string line;
    long long totalSectorID = 0;
    
    // Process each line from standard input.
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        
        // Find the last dash; everything before it is the encrypted name.
        std::size_t lastDashPos = line.rfind('-');
        if (lastDashPos == std::string::npos) {
            continue;
        }
        std::string encryptedName = line.substr(0, lastDashPos);
        
        // The remainder of the line contains the sector ID and checksum, e.g., "123[abxyz]".
        std::string sectorAndChecksum = line.substr(lastDashPos + 1);
        
        // Find the '[' which starts the checksum.
        std::size_t bracketPos = sectorAndChecksum.find('[');
        if (bracketPos == std::string::npos) {
            continue;
        }
        
        // Parse the sector ID.
        int sectorID = std::stoi(sectorAndChecksum.substr(0, bracketPos));
        
        // Extract the given checksum (exclude the closing ']').
        std::string givenChecksum = sectorAndChecksum.substr(bracketPos + 1, sectorAndChecksum.size() - bracketPos - 2);
        
        // Count the frequency of each letter in the encrypted name, ignoring dashes.
        std::array<int, 26> letterFreq = {0};
        for (char ch : encryptedName) {
            if (ch >= 'a' && ch <= 'z') {
                letterFreq[ch - 'a']++;
            }
        }
        
        // Create a vector with all letters a-z.
        std::vector<char> letters;
        letters.reserve(26);
        for (int i = 0; i < 26; ++i) {
            letters.push_back('a' + i);
        }
        
        // Sort letters by descending frequency and then alphabetically.
        std::sort(letters.begin(), letters.end(), [&](char a, char b) {
            if (letterFreq[a - 'a'] != letterFreq[b - 'a']) {
                return letterFreq[a - 'a'] > letterFreq[b - 'a'];
            }
            return a < b;
        });
        
        // Build the computed checksum from the first five letters.
        std::string computedChecksum(letters.begin(), letters.begin() + 5);
        
        // If the computed checksum matches the given checksum, add the sector ID to the total.
        if (computedChecksum == givenChecksum) {
            totalSectorID += sectorID;
        }
    }
    
    // Output the final sum of sector IDs.
    std::cout << totalSectorID << std::endl;
    return 0;
}

