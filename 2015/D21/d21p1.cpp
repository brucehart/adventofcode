#include <bits/stdc++.h>

struct Item {
    int cost;
    int damage;
    int armor;
};

// Function to calculate the number of turns to defeat the opponent
int turns_to_defeat(int hp, int damage_per_turn) {
    return std::ceil(static_cast<double>(hp) / damage_per_turn);
}

int main() {
    // Define the items available in the shop
    const std::vector<Item> weapons = {
        {8, 4, 0},      // Dagger
        {10, 5, 0},     // Shortsword
        {25, 6, 0},     // Warhammer
        {40, 7, 0},     // Longsword
        {74, 8, 0}      // Greataxe
    };

    const std::vector<Item> armors = {
        {0, 0, 0},      // No armor
        {13, 0, 1},     // Leather
        {31, 0, 2},     // Chainmail
        {53, 0, 3},     // Splintmail
        {75, 0, 4},     // Bandedmail
        {102, 0, 5}     // Platemail
    };

    const std::vector<Item> rings = {
        {0, 0, 0},      // No ring 1
        {0, 0, 0},      // No ring 2
        {25, 1, 0},     // Damage +1
        {50, 2, 0},     // Damage +2
        {100, 3, 0},    // Damage +3
        {20, 0, 1},     // Defense +1
        {40, 0, 2},     // Defense +2
        {80, 0, 3}      // Defense +3
    };

    // Read boss stats from stdin
    // Expected input format:
    // Hit Points: 103
    // Damage: 9
    // Armor: 2
    int boss_hp, boss_damage, boss_armor;
    std::string label;
    std::cin >> label >> label >> boss_hp;
    std::cin >> label >> boss_damage;
    std::cin >> label >> boss_armor;

    int player_hp = 100;
    int min_cost = INT32_MAX;

    // Iterate through all possible weapon, armor, and ring combinations
    for (const auto& weapon : weapons) {
        for (const auto& armor : armors) {
            for (size_t i = 0; i < rings.size(); ++i) {
                for (size_t j = i + 1; j < rings.size(); ++j) {
                    const auto& ring1 = rings[i];
                    const auto& ring2 = rings[j];

                    // Skip combinations where both rings are 'No ring'
                    if (i == 0 && j == 1) {
                        // This is allowed: no rings
                    }

                    // Calculate total cost, damage, and armor
                    int total_cost = weapon.cost + armor.cost + ring1.cost + ring2.cost;
                    int total_damage = weapon.damage + armor.damage + ring1.damage + ring2.damage;
                    int total_armor = weapon.armor + armor.armor + ring1.armor + ring2.armor;

                    // Calculate damage per turn, ensuring at least 1
                    int player_dpt = std::max(1, total_damage - boss_armor);
                    int boss_dpt = std::max(1, boss_damage - total_armor);

                    // Calculate turns to defeat
                    int turns_player = turns_to_defeat(boss_hp, player_dpt);
                    int turns_boss = turns_to_defeat(player_hp, boss_dpt);

                    // If player defeats boss in equal or fewer turns, it's a valid combination
                    if (turns_player <= turns_boss) {
                        min_cost = std::min(min_cost, total_cost);
                    }
                }
            }
        }
    }

    // Output the minimal cost to win
    std::cout << min_cost << std::endl;

    return 0;
}

