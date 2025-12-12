#include <iostream>
#include <string>

static int step_pos(int pos, int move) {
    pos = ((pos - 1 + move) % 10) + 1;
    return pos;
}

int main() {
    std::string line;
    int p1 = 0, p2 = 0;
    if (!std::getline(std::cin, line)) return 0;
    std::sscanf(line.c_str(), "Player 1 starting position: %d", &p1);
    if (!std::getline(std::cin, line)) return 0;
    std::sscanf(line.c_str(), "Player 2 starting position: %d", &p2);

    int s1 = 0, s2 = 0;
    int die = 1;
    long long rolls = 0;

    auto roll = [&]() {
        int v = die;
        die++;
        if (die == 101) die = 1;
        rolls++;
        return v;
    };

    while (s1 < 1000 && s2 < 1000) {
        int move = roll() + roll() + roll();
        p1 = step_pos(p1, move);
        s1 += p1;
        if (s1 >= 1000) break;

        move = roll() + roll() + roll();
        p2 = step_pos(p2, move);
        s2 += p2;
    }

    const long long losing = (s1 >= 1000) ? s2 : s1;
    std::cout << (losing * rolls) << std::endl;
    return 0;
}

