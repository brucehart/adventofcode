#include <iostream>
#include <string>
#include <algorithm>

// Function to increment the password
void incrementPassword(std::string& password) {
    for (int i = password.size() - 1; i >= 0; --i) {
        if (password[i] == 'z') {
            password[i] = 'a';
        } else {
            password[i]++;
            break;
        }
    }
}

// Function to check if the password contains a straight of three letters
bool hasStraight(const std::string& password) {
    for (size_t i = 0; i < password.size() - 2; ++i) {
        if (password[i + 1] == password[i] + 1 && password[i + 2] == password[i] + 2) {
            return true;
        }
    }
    return false;
}

// Function to check if the password contains forbidden letters
bool hasForbiddenLetters(const std::string& password) {
    return password.find_first_of("iol") != std::string::npos;
}

// Function to check if the password has at least two different non-overlapping pairs
bool hasTwoPairs(const std::string& password) {
    int pairCount = 0;
    char firstPairChar = '\0';
    for (size_t i = 0; i < password.size() - 1; ++i) {
        if (password[i] == password[i + 1]) {
            if (firstPairChar == '\0')
            {
                firstPairChar = password[i];
                pairCount++;
                i++;
            }
            else if (password[i] != firstPairChar)
            {
                pairCount++;
                i++;
            }
        }
    }
    return pairCount >= 2;
}

// Function to check if the password is valid
bool isValidPassword(const std::string& password) {
    return !hasForbiddenLetters(password) && hasStraight(password) && hasTwoPairs(password);
}

int main() {
    std::string password;
    std::cin >> password;

    do {
        incrementPassword(password);
    } while (!isValidPassword(password));
  
    std::cout << password << std::endl;

    return 0;
}
