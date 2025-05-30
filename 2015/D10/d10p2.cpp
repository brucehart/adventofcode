#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <string>
#include <map>

// Define types for convenience
typedef uint64_t ull;

struct Seq {
    std::string subsequence;
    std::vector<int> evolvesTo;
};

// Initialize the static map
// Reference table for look and say sequence: 
// https://www.nathanieljohnston.com/2010/10/a-derivation-of-conways-degree-71-look-and-say-polynomial/
static const std::map<int, Seq> seqMap = {
    {1, {"1112", {63}}},
    {2, {"1112133", {64, 62}}},
    {3, {"111213322112", {65}}},
    {4, {"111213322113", {66}}},
    {5, {"1113", {68}}},
    {6, {"11131", {69}}},
    {7, {"111311222112", {84, 55}}},
    {8, {"111312", {70}}},
    {9, {"11131221", {71}}},
    {10, {"1113122112", {76}}},
    {11, {"1113122113", {77}}},
    {12, {"11131221131112", {82}}},
    {13, {"111312211312", {78}}},
    {14, {"11131221131211", {79}}},
    {15, {"111312211312113211", {80}}},
    {16, {"111312211312113221133211322112211213322112", {81, 29, 91}}},
    {17, {"111312211312113221133211322112211213322113", {81, 29, 90}}},
    {18, {"11131221131211322113322112", {81, 30}}},
    {19, {"11131221133112", {75, 29, 92}}},
    {20, {"1113122113322113111221131221", {75, 32}}},
    {21, {"11131221222112", {72}}},
    {22, {"111312212221121123222112", {73}}},
    {23, {"111312212221121123222113", {74}}},
    {24, {"11132", {83}}},
    {25, {"1113222", {86}}},
    {26, {"1113222112", {87}}},
    {27, {"1113222113", {88}}},
    {28, {"11133112", {89, 92}}},
    {29, {"12", {1}}},
    {30, {"123222112", {3}}},
    {31, {"123222113", {4}}},
    {32, {"12322211331222113112211", {2, 61, 29, 85}}},
    {33, {"13", {5}}},
    {34, {"131112", {28}}},
    {35, {"13112221133211322112211213322112", {24, 33, 61, 29, 91}}},
    {36, {"13112221133211322112211213322113", {24, 33, 61, 29, 90}}},
    {37, {"13122112", {7}}},
    {38, {"132", {8}}},
    {39, {"13211", {9}}},
    {40, {"132112", {10}}},
    {41, {"1321122112", {21}}},
    {42, {"132112211213322112", {22}}},
    {43, {"132112211213322113", {23}}},
    {44, {"132113", {11}}},
    {45, {"1321131112", {19}}},
    {46, {"13211312", {12}}},
    {47, {"1321132", {13}}},
    {48, {"13211321", {14}}},
    {49, {"132113212221", {15}}},
    {50, {"13211321222113222112", {18}}},
    {51, {"1321132122211322212221121123222112", {16}}},
    {52, {"1321132122211322212221121123222113", {17}}},
    {53, {"13211322211312113211", {20}}},
    {54, {"1321133112", {6, 61, 29, 92}}},
    {55, {"1322112", {26}}},
    {56, {"1322113", {27}}},
    {57, {"13221133112", {25, 29, 92}}},
    {58, {"1322113312211", {25, 29, 67}}},
    {59, {"132211331222113112211", {25, 29, 85}}},
    {60, {"13221133122211332", {25, 29, 68, 61, 29, 89}}},
    {61, {"22", {61}}},
    {62, {"3", {33}}},
    {63, {"3112", {40}}},
    {64, {"3112112", {41}}},
    {65, {"31121123222112", {42}}},
    {66, {"31121123222113", {43}}},
    {67, {"3112221", {38, 39}}},
    {68, {"3113", {44}}},
    {69, {"311311", {48}}},
    {70, {"31131112", {54}}},
    {71, {"3113112211", {49}}},
    {72, {"3113112211322112", {50}}},
    {73, {"3113112211322112211213322112", {51}}},
    {74, {"3113112211322112211213322113", {52}}},
    {75, {"311311222", {47, 38}}},
    {76, {"311311222112", {47, 55}}},
    {77, {"311311222113", {47, 56}}},
    {78, {"3113112221131112", {47, 57}}},
    {79, {"311311222113111221", {47, 58}}},
    {80, {"311311222113111221131221", {47, 59}}},
    {81, {"31131122211311122113222", {47, 60}}},
    {82, {"3113112221133112", {47, 33, 61, 29, 92}}},
    {83, {"311312", {45}}},
    {84, {"31132", {46}}},
    {85, {"311322113212221", {53}}},
    {86, {"311332", {38, 29, 89}}},
    {87, {"3113322112", {38, 30}}},
    {88, {"3113322113", {38, 31}}},
    {89, {"312", {34}}},
    {90, {"312211322212221121123222113", {36}}},
    {91, {"312211322212221121123222112", {35}}},
    {92, {"32112", {37}}}
};

void updateState(std::vector<int>& state) {
    std::vector<int> newState(93, 0);

    for (int i = 1; i < 93; i++) { // Corrected range, index starts at 1
        if (state[i] == 0)
            continue;

        const auto& evolvesTo = seqMap.at(i).evolvesTo;
        for (int j : evolvesTo) {
            newState[j] += state[i];
        }
    }

    state = newState;
}

int main() {
    std::string startSeq;    
    std::cin >> startSeq;

    std::vector<int> state(93, 0);

    auto it = std::find_if(seqMap.begin(), seqMap.end(), [&startSeq](const auto& pair) {
        return pair.second.subsequence == startSeq;
    });

    if (it == seqMap.end()) {
        std::cerr << "Error: Starting sequence not found in map." << std::endl;
        return 1;
    }

    int startIndex = it->first;
    state[startIndex] = 1;

    for (int i = 0; i < 50; i++) {
        updateState(state);
    }

    ull totalLength = 0;
    for (int i = 1; i < 93; i++) { // Correct range, index starts at 1
        if (state[i] > 0) {
            totalLength += static_cast<ull>(state[i]) * seqMap.at(i).subsequence.length();
        }
    }

    std::cout << totalLength << std::endl;

    return 0;
}
