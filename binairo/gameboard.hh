/* Binairo
 *
 * Description:
 * This program is designed to implement the Binairo game,
 * which provides the user with a 6x6 grid. The grid allows
 * the placement of zeros, ones, or leaving a slot empty.
 * The objective is to fill the empty slots according to the following rules:
 *
 * Each row and column can have a maximum of three identical numbers.
 * Each row and column can have a maximum of two consecutive identical numbers.
 *
 * The program offers two options to start the game:
 *
 * Random Fill: The user is asked if they want to fill the grid with randomly
 * generated symbols. If the user chooses this option, they are also
 * required to provide a seed for the random number generator.
 *
 * User's Choice: The user can input their own 36 symbols into the grid.
 * Before accepting the input, the program checks whether the symbols
 * are valid or not.
 *
 * As the game progresses, the user can input symbol and its coordinates on
 * the game board each round. The game ends in a player's victory if the
 * game board is filled according to the rules. The program prevents additions
 * that violate the rules, but it is also possible for the player to reach
 * a point where no further additions are possible.
 *
 * The program checks the validity of inputs.
 * Input must follow these rules:
 *
 * The additional symbol must be zero or one.
 * Coordinates must be within the grid.
 * The indicated coordinate slot must be empty.
 *
 * */

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <vector>
#include <string>

// Type of the elements in the gameboard.
enum Element_type {ZERO, ONE, EMPTY};

// Constants for the size of the gameboard.
const int NUMBER_OF_SYMBOLS = 3;
const int SIZE = 2 * NUMBER_OF_SYMBOLS;

// Constant for the upper bound of probability distribution.
// Zeros and ones have the same probability, say x, and the estimated
// probability of empties is 6x, whereupon their sum is 8x, and thus,
// the interval must have eigth points, as [0..7].
const int DISTR_UPPER_BOUND = 7;

// Width of the left-most column, needed in printing the gameboard.
const unsigned int LEFT_COLUMN_WIDTH = 5;

class GameBoard
{
public:
    // Constructor.
    GameBoard();

    // Prints the gameboard.
    void print() const;

    // Checks if the game is over.
    // Returns true if the game is over, false otherwise.
    bool is_game_over();

    // Checks if the given seed is in the list of bad seeds.
    // Returns true if the seed is bad, false otherwise.
    bool is_bad_seed(unsigned int seed_value);

    // Creates a new row for the gameboard.
    // Parameters:
    //   row: Vector representing a row to be added to the gameboard.
    void create_board_row(std::vector<Element_type> row);

    // Checks if the specified coordinate on the gameboard is empty.
    // Parameters:
    //   y: Row coordinate.
    //   x: Column coordinate.
    // Returns true if the coordinate is empty, false otherwise.
    bool is_coordinate_empty(int y, int x);

    // Adds the specified fill symbol to the gameboard at the given coordinates.
    // Parameters:
    //   y: Row coordinate.
    //   x: Column coordinate.
    //   fill_symbol: Symbol to be added (ZERO, ONE).
    void add_fill_symbol(int y, int x, Element_type fill_symbol);

    // Checks if there are too many symbols horizontally at the specified position.
    // Parameters:
    //   y: Row coordinate.
    //   fill_symbol: Symbol to be checked (ZERO, ONE).
    // Returns true if there are too many symbols horizontally, false otherwise.
    bool too_many_symbols_horizontally(int y, Element_type fill_symbol);

    // Checks if there are too many symbols vertically at the given position.
    // Parameters:
    //   x: Column coordinate.
    //   fill_symbol: Symbol to be checked (ZERO, ONE).
    // Returns true if there are too many symbols vertically, false otherwise.
    bool too_many_symbols_vertically(int x, Element_type fill_symbol);

    // Checks if there are too many consecutive symbols at the given position.
    // Parameters:
    //   y: Row coordinate.
    //   x: Column coordinate.
    //   fill_symbol: Symbol to be checked (ZERO, ONE).
    // Returns true if there are too many consecutive symbols, false otherwise.
    bool too_many_symbols_consecutively(int y, int x, Element_type fill_symbol);


private:
    // Prints a line with the given length and the given character.
    void print_line(unsigned int length, char fill_character) const;

    // Seed values, from [1..50], producing non-solvable gameboard.
    const std::vector<unsigned int> BAD_SEEDS = { 2, 8, 12, 13, 16, 20, 21, 23,
                                                  26, 29, 31, 32, 34, 41, 42,
                                                  43, 44, 46 };
    std::vector<std::vector<Element_type>> board_;
};

#endif // GAMEBOARD_HH
