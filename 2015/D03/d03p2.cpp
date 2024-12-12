#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

void update_pos(int &x, int &y, const char c, std::map<std::pair<int, int>, bool>& visited){
    static const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    static const std::string arrows = ">v<^";
    auto d = directions[arrows.find(c)];
    x += d.first;
    y += d.second;
    visited[{x, y}] = true;
}

int main(){
    std::string input;
    std::getline(std::cin, input);
    std::map<std::pair<int, int>, bool> visited;
    int counter = 0;
    
    int santa_x = 0, santa_y = 0;
    int robo_x = 0, robo_y = 0;

    visited[{santa_x, santa_y}] = true;

    std::for_each(input.begin(), input.end(), [&](char c){        
        (counter ^= 1) ? update_pos(santa_x, santa_y, c, visited) : update_pos(robo_x, robo_y, c, visited);
    });

    std::cout << visited.size() << std::endl;    
}