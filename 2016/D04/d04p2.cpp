#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

// This program decrypts room names and finds the sector ID of the room
// where North Pole objects are stored.
// Each room line is in the format:
//   encrypted-name-sectorID[checksum]
// A room is considered real if its computed checksum (based on letter frequency)
// matches the given checksum.
// For real rooms, the encrypted name is decrypted using a shift cipher where
// each letter is rotated forward by the room's sector ID (mod 26). Dashes become spaces.
int main() {
    std::string line;
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        
        // Locate the last dash; the part before it is the encrypted name.
        std::size_t lastDashPos = line.rfind('-');
        if (lastDashPos == std::string::npos) {
            continue;
        }
        std::string encryptedName = line.substr(0, lastDashPos);
        
        // The remaining part contains the sector ID and checksum, e.g., "343[checksum]".
        std::string sectorAndChecksum = line.substr(lastDashPos + 1);
        std::size_t bracketPos = sectorAndChecksum.find('[');
        if (bracketPos == std::string::npos) {
            continue;
        }
        
        // Parse sector ID.
        int sectorID = std::stoi(sectorAndChecksum.substr(0, bracketPos));
        
        // Extract the provided checksum (exclude the closing bracket).
        std::string givenChecksum = sectorAndChecksum.substr(bracketPos + 1, sectorAndChecksum.size() - bracketPos - 2);
        
        // Count frequency of each letter in the encrypted name (ignore dashes).
        std::array<int, 26> letterFreq = {0};
        for (char ch : encryptedName) {
            if (ch >= 'a' && ch <= 'z') {
                letterFreq[ch - 'a']++;
            }
        }
        
        // Prepare a vector of letters a-z.
        std::vector<char> letters;
        letters.reserve(26);
        for (int i = 0; i < 26; ++i) {
            letters.push_back('a' + i);
        }
        
        // Sort letters by descending frequency, then alphabetically.
        std::sort(letters.begin(), letters.end(), [&](char a, char b) {
            if (letterFreq[a - 'a'] != letterFreq[b - 'a']) {
                return letterFreq[a - 'a'] > letterFreq[b - 'a'];
            }
            return a < b;
        });
        
        // Build the computed checksum from the first five letters.
        std::string computedChecksum(letters.begin(), letters.begin() + 5);
        
        // Only consider real rooms.
        if (computedChecksum != givenChecksum) {
            continue;
        }
        
        // Decrypt the encrypted name:
        // - Letters are shifted forward by sectorID positions (wrap-around via modulo 26).
        // - Dashes become spaces.
        std::string decrypted;
        decrypted.reserve(encryptedName.size());
        for (char ch : encryptedName) {
            if (ch == '-') {
                decrypted.push_back(' ');
            } else if (ch >= 'a' && ch <= 'z') {
                // Shift letter: subtract 'a', add sectorID, take modulo 26, then add 'a'
                char decChar = 'a' + ((ch - 'a' + sectorID) % 26);
                decrypted.push_back(decChar);
            }
        }
        
        // Check if the decrypted name contains "northpole"
        if (decrypted.find("northpole") != std::string::npos) {
            std::cout << sectorID << std::endl;
            return 0;
        }
    }
    
    return 0;
}

