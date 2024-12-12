#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

int main(){
    std::string input;
    std::getline(std::cin, input);
    std::map<std::pair<int, int>, bool> visited;

    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::string arrows = ">v<^";
    int x = 0, y = 0;
    visited[{x, y}] = true;

    std::for_each(input.begin(), input.end(), [&](char c){        
        auto d = directions[arrows.find(c)];
        x += d.first;
        y += d.second;
        visited[{x, y}] = true;
    });

    std::cout << visited.size() << std::endl;    
}