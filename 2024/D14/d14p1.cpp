#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>

static const int NUM_COLS = 101;
static const int NUM_ROWS = 103;
static const int NUM_STEPS = 100;

struct Robot {
    int pos_x;
    int pos_y;
    int velocity_x;
    int velocity_y;

    void moveRobot(int numSteps)
    {
        pos_x = ((pos_x + numSteps * velocity_x) % NUM_COLS + NUM_COLS) % NUM_COLS;
        pos_y = ((pos_y + numSteps * velocity_y) % NUM_ROWS + NUM_ROWS) % NUM_ROWS;        
    }
};

std::vector<Robot> readData() {
    std::vector<Robot> robots;
    std::string line;
    std::regex pattern(R"(p=(\-?\d+),(\-?\d+) v=(\-?\d+),(\-?\d+))");
    std::smatch match;

    while (std::getline(std::cin, line)) {
        if (std::regex_search(line, match, pattern)) {
            Robot robot;
            robot.pos_x = std::stoi(match[1]);
            robot.pos_y = std::stoi(match[2]);
            robot.velocity_x = std::stoi(match[3]);
            robot.velocity_y = std::stoi(match[4]);
            robots.push_back(robot);
        } else {
            std::cerr << "Warning: Line does not match the expected format: " << line << std::endl;
        }
    }

    return robots;
}



int main() {
    std::vector<Robot> robots = readData();
    std::for_each(robots.begin(), robots.end(), [](Robot& robot) { robot.moveRobot(NUM_STEPS);});

    std::vector<int> quadCount = {0,0,0,0};
    
    for (const Robot& robot : robots) {
        if (robot.pos_x < NUM_COLS / 2 && robot.pos_y < NUM_ROWS / 2) {
                quadCount[0]++;
        } else if (robot.pos_x > NUM_COLS / 2 && robot.pos_y < NUM_ROWS / 2) {
                quadCount[1]++;
        } else if (robot.pos_x < NUM_COLS / 2 && robot.pos_y > NUM_ROWS / 2) {
                quadCount[2]++;
        } else if (robot.pos_x > NUM_COLS / 2 && robot.pos_y > NUM_ROWS / 2) {
                quadCount[3]++;
        }
    }


    int quadProduct = quadCount[0] * quadCount[1] * quadCount[2] * quadCount[3];
    std::cout << quadProduct << std::endl;
   
    return 0;
}
