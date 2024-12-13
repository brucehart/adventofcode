#include <bits/stdc++.h>

// Structure to represent an item with cost, damage, and armor attributes
struct Item {
    int cost;
    int damage;
    int armor;
};

// Function to calculate the number of turns required to defeat the opponent
int turns_to_defeat(int hp, int damage_per_turn) {
    // Ceiling division to account for partial turns
    return static_cast<int>(std::ceil(static_cast<double>(hp) / damage_per_turn));
}

int main() {
    // Define the items available in the shop

    // Weapons: Player must choose exactly one
    const std::vector<Item> weapons = {
        {8, 4, 0},      // Dagger
        {10, 5, 0},     // Shortsword
        {25, 6, 0},     // Warhammer
        {40, 7, 0},     // Longsword
        {74, 8, 0}      // Greataxe
    };

    // Armors: Player can choose at most one (including no armor)
    const std::vector<Item> armors = {
        {0, 0, 0},      // No armor
        {13, 0, 1},     // Leather
        {31, 0, 2},     // Chainmail
        {53, 0, 3},     // Splintmail
        {75, 0, 4},     // Bandedmail
        {102, 0, 5}     // Platemail
    };

    // Rings: Player can choose up to two rings (including no rings)
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

    // Read boss stats from standard input
    // Expected input format:
    // Hit Points: 103
    // Damage: 9
    // Armor: 2
    int boss_hp, boss_damage, boss_armor;
    std::string label;

    // Read "Hit", "Points:", and the value
    std::cin >> label >> label >> boss_hp;
    // Read "Damage:" and the value
    std::cin >> label >> boss_damage;
    // Read "Armor:" and the value
    std::cin >> label >> boss_armor;

    // Player's fixed hit points
    const int player_hp = 100;
    // Initialize maximum cost to track the most gold spent resulting in a loss
    int max_cost = 0;

    // Iterate through all possible combinations of weapons, armors, and rings
    for (const auto& weapon : weapons) {
        for (const auto& armor : armors) {
            for (size_t i = 0; i < rings.size(); ++i) {
                for (size_t j = i + 1; j < rings.size(); ++j) {
                    const auto& ring1 = rings[i];
                    const auto& ring2 = rings[j];

                    // Calculate total cost, damage, and armor for the current combination
                    int total_cost = weapon.cost + armor.cost + ring1.cost + ring2.cost;
                    int total_damage = weapon.damage + armor.damage + ring1.damage + ring2.damage;
                    int total_armor = weapon.armor + armor.armor + ring1.armor + ring2.armor;

                    // Calculate damage per turn, ensuring at least 1 damage
                    int player_dpt = std::max(1, total_damage - boss_armor);
                    int boss_dpt = std::max(1, boss_damage - total_armor);

                    // Calculate the number of turns required for both to defeat each other
                    int turns_player = turns_to_defeat(boss_hp, player_dpt);
                    int turns_boss = turns_to_defeat(player_hp, boss_dpt);

                    /*
                        Condition for losing:
                        If the player requires more turns to defeat the boss than the boss requires
                        to defeat the player, the player loses.
                    */
                    if (turns_player > turns_boss) {
                        max_cost = std::max(max_cost, total_cost);
                    }
                }
            }
        }
    }

    // Output the maximal cost to lose
    std::cout << max_cost << std::endl;

    return 0;
}

