#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <optional>
#include <cmath>
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>

// Struct representing a hailstone with position and velocity vectors
struct Hailstone {
    std::array<double, 3> position;
    std::array<double, 3> velocity;
};

// Struct representing the solution with position and velocity vectors
struct Solution {
    std::array<double, 3> position;
    std::array<double, 3> velocity;
};

// Computes the cross product of two 3D vectors
std::array<double, 3> compute_cross_product(const std::array<double, 3>& vectorA, const std::array<double, 3>& vectorB) {
    return {
        vectorA[1] * vectorB[2] - vectorA[2] * vectorB[1],
        vectorA[2] * vectorB[0] - vectorA[0] * vectorB[2],
        vectorA[0] * vectorB[1] - vectorA[1] * vectorB[0]
    };
}

// Appends vector 'bVector' as the last column of the matrix
void append_augmented_column(std::vector<std::vector<double>>& matrix, const std::vector<double>& bVector) {
    for (size_t rowIndex = 0; rowIndex < matrix.size(); ++rowIndex) {
        matrix[rowIndex].push_back(bVector[rowIndex]);
    }
}

// Performs Gaussian elimination with partial pivoting
// Returns true if the matrix is singular, otherwise false
bool perform_gaussian_elimination(std::vector<std::vector<double>>& augmentedMatrix) {
    size_t numRows = augmentedMatrix.size();
    for (size_t pivotRow = 0; pivotRow < numRows; ++pivotRow) {
        // Partial pivoting to enhance numerical stability
        size_t maxRow = pivotRow;
        double maxValue = std::abs(augmentedMatrix[pivotRow][pivotRow]);
        for (size_t currentRow = pivotRow + 1; currentRow < numRows; ++currentRow) {
            if (std::abs(augmentedMatrix[currentRow][pivotRow]) > maxValue) {
                maxValue = std::abs(augmentedMatrix[currentRow][pivotRow]);
                maxRow = currentRow;
            }
        }

        // Check for singularity
        if (maxValue == 0.0) {
            return true;
        }

        // Swap the current pivot row with the row containing the maximum element
        if (maxRow != pivotRow) {
            std::swap(augmentedMatrix[pivotRow], augmentedMatrix[maxRow]);
        }

        // Eliminate entries below the pivot
        for (size_t targetRow = pivotRow + 1; targetRow < numRows; ++targetRow) {
            double eliminationFactor = augmentedMatrix[targetRow][pivotRow] / augmentedMatrix[pivotRow][pivotRow];
            for (size_t col = pivotRow; col <= numRows; ++col) {
                augmentedMatrix[targetRow][col] -= eliminationFactor * augmentedMatrix[pivotRow][col];
            }
            augmentedMatrix[targetRow][pivotRow] = 0.0;
        }
    }
    return false;
}

// Performs back substitution on the upper triangular matrix
std::vector<double> perform_back_substitution(const std::vector<std::vector<double>>& augmentedMatrix) {
    size_t numRows = augmentedMatrix.size();
    std::vector<double> solution(numRows, 0.0);
    for (int row = static_cast<int>(numRows) - 1; row >= 0; --row) {
        double sum = 0.0;
        for (size_t col = row + 1; col < numRows; ++col) {
            sum += augmentedMatrix[row][col] * solution[col];
        }
        solution[row] = (augmentedMatrix[row][numRows] - sum) / augmentedMatrix[row][row];
    }
    return solution;
}

// Constructs the augmented matrix based on selected hailstones
std::vector<std::vector<double>> build_augmented_matrix(
    const std::vector<Hailstone>& selectedHailstones,
    std::vector<double>& bVector
) {
    std::vector<std::vector<double>> matrix;
    bVector.clear();
    size_t numberOfPairs = selectedHailstones.size() - 1;

    for (size_t i = 0; i < numberOfPairs; ++i) {
        size_t j = i + 1;
        const Hailstone& hailA = selectedHailstones[i];
        const Hailstone& hailB = selectedHailstones[j];

        // Calculate the difference of cross products
        std::array<double, 3> crossA = compute_cross_product(hailA.position, hailA.velocity);
        std::array<double, 3> crossB = compute_cross_product(hailB.position, hailB.velocity);
        std::vector<double> crossDifference(3);
        std::transform(crossA.begin(), crossA.end(), crossB.begin(), crossDifference.begin(), std::minus<double>());
        std::reverse(crossDifference.begin(), crossDifference.end());
        bVector.insert(bVector.end(), crossDifference.begin(), crossDifference.end());

        // First equation set
        std::vector<double> equation1 = {
            hailA.velocity[1] - hailB.velocity[1],
            -(hailA.velocity[0] - hailB.velocity[0]),
            0.0,
            -(hailA.position[1] - hailB.position[1]),
            hailA.position[0] - hailB.position[0],
            0.0
        };
        matrix.emplace_back(equation1);

        // Second equation set
        std::vector<double> equation2 = {
            -(hailA.velocity[2] - hailB.velocity[2]),
            0.0,
            hailA.velocity[0] - hailB.velocity[0],
            (hailA.position[2] - hailB.position[2]),
            0.0,
            -(hailA.position[0] - hailB.position[0])
        };
        matrix.emplace_back(equation2);

        // Third equation set
        std::vector<double> equation3 = {
            0.0,
            hailA.velocity[2] - hailB.velocity[2],
            -(hailA.velocity[1] - hailB.velocity[1]),
            0.0,
            -(hailA.position[2] - hailB.position[2]),
            hailA.position[1] - hailB.position[1]
        };
        matrix.emplace_back(equation3);
    }

    // Append the b vector to the matrix as the augmented column
    append_augmented_column(matrix, bVector);
    return matrix;
}

// Attempts to solve the system for selected hailstone indices
// Returns an optional Solution if solvable, otherwise std::nullopt
std::optional<Solution> solve_system(
    const std::vector<Hailstone>& hailstones,
    size_t index1,
    size_t index2,
    size_t index3
) {
    // Ensure all indices are unique
    if (index1 == index2 || index1 == index3 || index2 == index3) {
        return std::nullopt;
    }

    // Select the hailstones based on provided indices
    std::vector<size_t> selectedIndices = { index1, index2, index3 };
    std::vector<Hailstone> selectedHailstones;
    selectedHailstones.reserve(3);
    for (size_t idx : selectedIndices) {
        selectedHailstones.push_back(hailstones[idx]);
    }

    // Build the augmented matrix for the system of equations
    std::vector<double> bVector;
    std::vector<std::vector<double>> augmentedMatrix = build_augmented_matrix(selectedHailstones, bVector);

    // Perform Gaussian elimination
    bool isSingular = perform_gaussian_elimination(augmentedMatrix);
    if (isSingular) {
        return std::nullopt;
    }

    // Perform back substitution to find the solution
    std::vector<double> solutionValues = perform_back_substitution(augmentedMatrix);

    // Ensure the solution has enough variables
    if (solutionValues.size() < 6) {
        return std::nullopt;
    }

    // Populate the Solution struct with the obtained values
    Solution solution;
    std::copy_n(solutionValues.begin(), 3, solution.position.begin());
    std::copy_n(solutionValues.begin() + 3, 3, solution.velocity.begin());

    return solution;
}

// Renamed function from 'solve_two' to 'solve'
// Solves for the collision and returns the sum of the solution's position components
double solve(const std::vector<Hailstone>& hailstones, const std::vector<int>& /* limits */) {
    const size_t totalHailstones = hailstones.size();
    const size_t requiredHailstones = 3;

    // Initialize random number generator with a fixed seed based on the string "aoc"
    std::hash<std::string> hasher;
    std::mt19937 rng(static_cast<unsigned int>(hasher("hohoho")));
    std::uniform_int_distribution<size_t> distribution(0, totalHailstones - 1);

    while (true) {
        // Randomly select three hailstone indices
        size_t firstIndex = distribution(rng);
        size_t secondIndex = distribution(rng);
        size_t thirdIndex = distribution(rng);

        // Attempt to solve the system with the selected indices
        std::optional<Solution> solution = solve_system(hailstones, firstIndex, secondIndex, thirdIndex);
        if (solution.has_value()) {
            // Calculate and return the sum of the position components
            double positionSum = std::accumulate(solution->position.begin(), solution->position.end(), 0.0);
            return positionSum;
        }
        // If no solution is found, continue the loop
    }
}

int main() {
    std::vector<Hailstone> hailstones;
    double px, py, pz, vx, vy, vz;
    char comma, at;

    // Read hailstone data from input in the format: px,py,pz@vx,vy,vz
    while (std::cin >> px >> comma >> py >> comma >> pz >> at >> vx >> comma >> vy >> comma >> vz) {
        hailstones.push_back({ {px, py, pz}, {vx, vy, vz} });
    }

    // Call the solve function to compute the sum of position components
    double result = solve(hailstones, {});

    // Output the result
    std::cout << std::fixed << std::setprecision(0) << std::round(result) << std::endl;

    return 0;
}
