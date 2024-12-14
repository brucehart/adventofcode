#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static const int NUM_COLS = 101; // Width (W)
static const int NUM_ROWS = 103; // Height (H)

struct Robot {
    int pos_x;
    int pos_y;
    int velocity_x;
    int velocity_y;

    void moveRobot(int t) {
        pos_x = ((pos_x + t * velocity_x) % NUM_COLS + NUM_COLS) % NUM_COLS;
        pos_y = ((pos_y + t * velocity_y) % NUM_ROWS + NUM_ROWS) % NUM_ROWS;
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

// Function to calculate variance of a vector
double calculateVariance(const std::vector<int>& values) {
    double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    double variance = 0.0;
    for (int value : values) {
        variance += (value - mean) * (value - mean);
    }
    return variance / values.size();
}

// Modular inverse function (based on extended Euclidean algorithm)
int modularInverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;

        // Update m and a
        m = a % m, a = t;

        // Update x0 and x1
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

// Function to generate an image of robot positions
void generateImage(const std::vector<Robot>& robots, int scale, const std::string& filename) {
    int width = scale * NUM_COLS;
    int height = scale * NUM_ROWS;
    std::vector<unsigned char> image(width * height, 0); // Initialize black background

    for (const auto& robot : robots) {
        int start_x = robot.pos_x * scale;
        int start_y = robot.pos_y * scale;

        for (int dx = 0; dx < scale; ++dx) {
            for (int dy = 0; dy < scale; ++dy) {
                int x = start_x + dx;
                int y = start_y + dy;
                if (x < width && y < height) {
                    image[y * width + x] = 255; // Set pixel to white
                }
            }
        }
    }

    // Write the image to a file
    if (!stbi_write_png(filename.c_str(), width, height, 1, image.data(), width)) {
        std::cerr << "Error: Failed to write image to " << filename << std::endl;
    } else {
        //std::cout << "Image saved to " << filename << std::endl;
    }
}

int main() {
    std::vector<Robot> robots = readData();
    int bx = 0, by = 0; // Times for minimal variance
    double bxvar = NUM_COLS * 1000, byvar = NUM_ROWS * 1000; // Initial high variance values

    for (int t = 0; t < std::max(NUM_COLS, NUM_ROWS); t++) {
        std::vector<int> xs, ys;

        // Simulate robot positions at time t
        for (const auto& robot : robots) {
            Robot temp = robot;
            temp.moveRobot(t);
            xs.push_back(temp.pos_x);
            ys.push_back(temp.pos_y);
        }

        // Calculate variances
        double xvar = calculateVariance(xs);
        double yvar = calculateVariance(ys);

        if (xvar < bxvar) {
            bx = t;
            bxvar = xvar;
        }
        if (yvar < byvar) {
            by = t;
            byvar = yvar;
        }
    }

    // Modular arithmetic calculation for part 2
    int W = NUM_COLS;
    int H = NUM_ROWS;
    int modular_inverse_W = modularInverse(W, H);
    int t_result = bx + ((modular_inverse_W * (by - bx)) % H) * W;

    std::cout << t_result << std::endl;

    // Generate image of robot positions at t_result
    for (auto& robot : robots) {
        robot.moveRobot(t_result);
    }

    generateImage(robots, 4, "christmas-tree.png");

    return 0;
}
