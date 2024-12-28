#include <iostream>

// Entry point of the program
int main() {
    // Initialize total score to accumulate the scores of all rounds
    long long total_score = 0;

    char opponent_char, outcome_char;

    // Read input until end of file (EOF)
    // Each round consists of two characters: opponent's move and desired outcome
    while (std::cin >> opponent_char >> outcome_char) {
        // Map opponent's move from 'A', 'B', 'C' to 0 (Rock), 1 (Paper), 2 (Scissors)
        int opponent_move = opponent_char - 'A';

        // Determine the desired outcome and corresponding shift
        // 'X' means you need to lose: shift = 2
        // 'Y' means you need to draw: shift = 0
        // 'Z' means you need to win: shift = 1
        int shift;
        int outcome_score;

        switch (outcome_char) {
            case 'X': // Need to lose
                shift = 2;
                outcome_score = 0;
                break;
            case 'Y': // Need to draw
                shift = 0;
                outcome_score = 3;
                break;
            case 'Z': // Need to win
                shift = 1;
                outcome_score = 6;
                break;
            default:
                // Invalid input character, skip this round
                continue;
        }

        // Calculate your move to achieve the desired outcome
        // (opponent_move + shift) modulo 3 ensures the move cycles correctly
        int your_move = (opponent_move + shift) % 3;

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

