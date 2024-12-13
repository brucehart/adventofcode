#include <bits/stdc++.h>
#include <functional>

// Structure to represent the game state
struct GameState {
    int player_hp;
    int player_mana;
    int boss_hp;
    int boss_damage;
    int player_armor;
    int mana_spent;
    // Effect timers
    int shield_timer;
    int poison_timer;
    int recharge_timer;
};

// Function to apply active effects at the start of a turn
void apply_effects(GameState &state) {
    // Reset armor before applying effects
    state.player_armor = 0;
    
    // Apply Shield effect
    if (state.shield_timer > 0) {
        state.player_armor = 7;
        state.shield_timer--;
    }

    // Apply Poison effect
    if (state.poison_timer > 0) {
        state.boss_hp -= 3;
        state.poison_timer--;
    }

    // Apply Recharge effect
    if (state.recharge_timer > 0) {
        state.player_mana += 101;
        state.recharge_timer--;
    }
}

// Recursive function to simulate the game and find the minimum mana to win
bool dfs(GameState state, int &min_mana_spent) {
    // *** Part Two Modification: Hard Mode ***
    // Player loses 1 HP at the start of their turn
    state.player_hp -= 1;
    if (state.player_hp <= 0) {
        // Player dies before taking any other actions
        return false;
    }

    // Apply effects at the start of the player's turn
    apply_effects(state);

    // Check if boss is defeated after effects
    if (state.boss_hp <= 0) {
        min_mana_spent = std::min(min_mana_spent, state.mana_spent);
        return true;
    }

    // Define available spells
    struct Spell {
        std::string name;
        int cost;
        // Function to apply spell effects
        std::function<void(GameState &)> cast;
    };

    std::vector<Spell> spells = {
        {"Magic Missile", 53, [&](GameState &s) {
            s.boss_hp -= 4;
        }},
        {"Drain", 73, [&](GameState &s) {
            s.boss_hp -= 2;
            s.player_hp += 2;
        }},
        {"Shield", 113, [&](GameState &s) {
            s.shield_timer = 6;
        }},
        {"Poison", 173, [&](GameState &s) {
            s.poison_timer = 6;
        }},
        {"Recharge", 229, [&](GameState &s) {
            s.recharge_timer = 5;
        }}
    };

    bool can_win = false;

    // Iterate through all spells to try casting them
    for (const auto &spell : spells) {
        // Check if enough mana to cast the spell
        if (state.player_mana < spell.cost)
            continue;

        // Check if the spell's effect is already active
        if ((spell.name == "Shield" && state.shield_timer > 0) ||
            (spell.name == "Poison" && state.poison_timer > 0) ||
            (spell.name == "Recharge" && state.recharge_timer > 0))
            continue;

        // Prune paths where mana spent already exceeds current minimum
        if (state.mana_spent + spell.cost >= min_mana_spent)
            continue;

        // Create a new state for the next turn
        GameState next_state = state;
        next_state.player_mana -= spell.cost;
        next_state.mana_spent += spell.cost;

        // Cast the spell
        spell.cast(next_state);

        // Check if boss is defeated after casting the spell
        if (next_state.boss_hp <= 0) {
            min_mana_spent = std::min(min_mana_spent, next_state.mana_spent);
            can_win = true;
            continue;
        }

        // *** Boss's Turn ***
        // Apply effects at the start of the boss's turn
        apply_effects(next_state);

        // Check if boss is defeated after effects
        if (next_state.boss_hp <= 0) {
            min_mana_spent = std::min(min_mana_spent, next_state.mana_spent);
            can_win = true;
            continue;
        }

        // Boss attacks
        int damage = std::max(1, next_state.boss_damage - next_state.player_armor);
        next_state.player_hp -= damage;

        // Check if player is defeated
        if (next_state.player_hp <= 0)
            continue;

        // Continue the game recursively if mana spent is still less than current minimum
        if (next_state.mana_spent < min_mana_spent) {
            if (dfs(next_state, min_mana_spent))
                can_win = true;
        }
    }

    return can_win;
}

int main() {
    // Read boss stats from stdin
    int boss_hp = 0, boss_damage = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.find("Hit Points:") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                boss_hp = std::stoi(line.substr(pos + 1));
            }
        } else if (line.find("Damage:") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                boss_damage = std::stoi(line.substr(pos + 1));
            }
        }
    }

    // Initialize the game state
    GameState initial_state = {50, 500, boss_hp, boss_damage, 0, 0, 0, 0, 0};
    int min_mana_spent = std::numeric_limits<int>::max();

    // Start the DFS search
    if (dfs(initial_state, min_mana_spent)) {
        std::cout << min_mana_spent << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}

