#include <iostream>

// Entry point of the program
int main() {
    // Initialize total score to accumulate the scores of all rounds
    long long total_score = 0;

    char opponent_char, your_char;

    // Read input until end of file (EOF)
    // Each round consists of two characters: opponent's move and your move
    while (std::cin >> opponent_char >> your_char) {
        // Map opponent's move from 'A', 'B', 'C' to 0 (Rock), 1 (Paper), 2 (Scissors)
        int opponent_move = opponent_char - 'A';

        // Map your move from 'X', 'Y', 'Z' to 0 (Rock), 1 (Paper), 2 (Scissors)
        int your_move = your_char - 'X';

        // Determine the outcome of the round
        // Calculate the difference modulo 3 to handle the cyclical nature of the game
        // diff == 0 => Draw
        // diff == 1 => You win
        // diff == 2 => You lose
        int diff = (your_move - opponent_move + 3) % 3;

        int outcome_score;
        if (diff == 0) {
            // Draw: add 3 points
            outcome_score = 3;
        } else if (diff == 1) {
            // Win: add 6 points
            outcome_score = 6;
        } else {
            // Loss: add 0 points
            outcome_score = 0;
        }

        // Calculate the score for the shape you selected
        // Rock: 1 point, Paper: 2 points, Scissors: 3 points
        int shape_score = your_move + 1;

        // Add the round's total score to the cumulative total score
        total_score += (shape_score + outcome_score);
    }

    // Output the final total score to standard output
    std::cout << total_score << std::endl;

    return 0;
}

