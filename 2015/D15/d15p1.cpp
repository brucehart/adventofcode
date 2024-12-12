#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Structure to hold properties of each ingredient
struct Ingredient {
    std::string name;
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
};

// Function to parse a single line of input into an Ingredient
Ingredient parse_ingredient(const std::string& line) {
    Ingredient ingredient;
    std::stringstream ss(line);
    std::string token;

    // Parse the name (before the colon)
    std::getline(ss, ingredient.name, ':');

    // Parse the properties
    while (std::getline(ss, token, ',')) {
        std::stringstream prop_ss(token);
        std::string property;
        int value;

        // Extract property name and value
        prop_ss >> property;
        prop_ss.ignore(1); // Ignore space or ':'
        prop_ss >> value;

        // Assign the value to the correct property
        if (property == "capacity") {
            ingredient.capacity = value;
        } else if (property == "durability") {
            ingredient.durability = value;
        } else if (property == "flavor") {
            ingredient.flavor = value;
        } else if (property == "texture") {
            ingredient.texture = value;
        } else if (property == "calories") {
            ingredient.calories = value;
        }
    }

    // Handle the last property after the last comma
    if (ss >> token) {
        std::stringstream prop_ss(token);
        std::string property;
        int value;

        prop_ss >> property;
        prop_ss.ignore(1); // Ignore space
        prop_ss >> value;

        if (property == "calories") {
            ingredient.calories = value;
        }
    }

    return ingredient;
}

// Function to compute the total score given a distribution of teaspoons
long long compute_score(const std::vector<Ingredient>& ingredients, const std::vector<int>& amounts) {
    int total_capacity = 0;
    int total_durability = 0;
    int total_flavor = 0;
    int total_texture = 0;

    // Aggregate the properties
    for (size_t i = 0; i < ingredients.size(); ++i) {
        total_capacity += ingredients[i].capacity * amounts[i];
        total_durability += ingredients[i].durability * amounts[i];
        total_flavor += ingredients[i].flavor * amounts[i];
        total_texture += ingredients[i].texture * amounts[i];
    }

    // Replace negative totals with zero
    total_capacity = std::max(0, total_capacity);
    total_durability = std::max(0, total_durability);
    total_flavor = std::max(0, total_flavor);
    total_texture = std::max(0, total_texture);

    // Compute the total score as the product of the properties
    return static_cast<long long>(total_capacity) * total_durability * total_flavor * total_texture;
}

int main() {
    std::vector<Ingredient> ingredients;
    std::string line;

    // Read and parse each line from standard input
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            ingredients.push_back(parse_ingredient(line));
        }
    }

    // Number of ingredients
    size_t num_ingredients = ingredients.size();
    assert(num_ingredients > 0 && "There must be at least one ingredient.");

    // Vector to hold the amount of each ingredient
    std::vector<int> amounts(num_ingredients, 0);

    // Variable to keep track of the maximum score
    long long max_score = 0;

    // Recursive lambda to explore all possible distributions of teaspoons
    std::function<void(size_t, int)> backtrack = [&](size_t index, int remaining) {
        if (index == num_ingredients - 1) {
            // Assign the remaining teaspoons to the last ingredient
            amounts[index] = remaining;
            // Compute the score for the current distribution
            long long current_score = compute_score(ingredients, amounts);
            // Update the maximum score if necessary
            if (current_score > max_score) {
                max_score = current_score;
            }
            return;
        }

        // Iterate through all possible amounts for the current ingredient
        for (int i = 0; i <= remaining; ++i) {
            amounts[index] = i;
            backtrack(index + 1, remaining - i);
        }
    };

    // Start the backtracking with the first ingredient and 100 teaspoons
    backtrack(0, 100);

    // Output the maximum score
    std::cout << max_score << std::endl;

    return 0;
}
