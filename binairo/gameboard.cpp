#include "gameboard.hh"
#include <iostream>
#include <random>
#include <map>

GameBoard::GameBoard()
{

}

void GameBoard::print() const
{
    // Printing upper border
    print_line(LEFT_COLUMN_WIDTH + 1 + 2 * SIZE + 1, '=');

    // Printing title row
    std::cout << "|   | ";
    for(unsigned int i = 0; i < SIZE; ++i)
    {
        std::cout << i + 1 << " ";
    }
    std::cout << "|" << std::endl;

    // Printing line after the title row
    print_line(LEFT_COLUMN_WIDTH + 1 + 2 * SIZE + 1, '-');

    // Printing the actual content of the gameboard
    for(unsigned int i = 0; i < SIZE; ++i)
    {
        std::cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < SIZE; ++j)
        {
            if(board_.at(i).at(j) == ZERO){
                std::cout << "0 ";
            } else if (board_.at(i).at(j) == ONE){
                std::cout << "1 ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << "|" << std::endl;
    }

    // Printing lower border
    print_line(LEFT_COLUMN_WIDTH + 1 + 2 * SIZE + 1, '=');
}

void GameBoard::print_line(unsigned int length, char fill_character) const
{
    for(unsigned int i = 0; i < length; ++i)
    {
        std::cout << fill_character;
    }
    std::cout << std::endl;
}

bool GameBoard::is_game_over(){
    // Checks if the game is over by looping through the game board
    for (unsigned int j = 0; j < SIZE; j++){
        for (unsigned int i = 0; i < SIZE; i++){
            // If an empty value is found, the game is not over
            if (board_.at(j).at(i) == EMPTY){
                return false;
            }
        }
    }
    // If non-empty rows are found, the game is over
    return true;
}

bool GameBoard::is_bad_seed(unsigned int seed_value){
    // Checks if the given seed value is in the list of bad seeds
    for (unsigned int badSeed : BAD_SEEDS){
        if (seed_value == badSeed){
            std::cout << "Bad seed" << std::endl;
            return true;
        }
    }
    // If the seed value is not in the list, it is not a bad seed
    return false;
}

void GameBoard::add_fill_symbol(int y, int x, Element_type fill_symbol){
    // Add the specified fill symbol to the game board at the given coordinates
    if (fill_symbol == ZERO) {
        board_.at(y - 1).at(x - 1) = ZERO;
    } else if (fill_symbol == ONE) {
        board_.at(y - 1).at(x - 1) = ONE;
    }
}

bool GameBoard::too_many_symbols_vertically(int x, Element_type fill_symbol){

    // Define variable for the amount of zeros in a vertical row
    int zeros_amount = 0;

    // Define variable for the amount of zeros in a vertical row
    int ones_amount= 0;

    // Increment the count based on the fill symbol
    if (fill_symbol == ZERO) {
        zeros_amount++;
    } else if (fill_symbol == ONE) {
        ones_amount++;
    }

    // Loops through the row to count the number of zeros and ones
    for(unsigned int j = 0; j < SIZE; ++j){
        if (board_.at(j).at(x-1) == ZERO){
            zeros_amount++;
        } else if (board_.at(j).at(x-1) == ONE){
            ones_amount++;
        }
    }

    // Check if the total count of zeros or ones exceeds the allowed limit
    return (ones_amount > 3 || zeros_amount > 3);
}

bool GameBoard::too_many_symbols_horizontally(int y, Element_type fill_symbol){

    // Define variable for the amount of zeros in a horizontal row
    int zeros_amount = 0;

    // Define variable for the amount of zeros in a horizontal row
    int ones_amount= 0;

    // Increment the count based on the fill symbol
    if (fill_symbol == ZERO) {
        zeros_amount++;
    } else if (fill_symbol == ONE) {
        ones_amount++;
    }

    // Loops through the row to count the number of zeros and ones
    for(unsigned int j = 0; j < SIZE; ++j){
        if (board_.at(y-1).at(j) == ZERO){
            zeros_amount++;
        } else if (board_.at(y-1).at(j) == ONE){
            ones_amount++;
        }
    }

    // Check if the total count of zeros or ones exceeds the allowed limit
    return (ones_amount > 3 || zeros_amount > 3);
}

bool GameBoard::too_many_symbols_consecutively(int y, int x,
                                               Element_type fill_symbol){
        // Add symbol to game board
        board_.at(y - 1).at(x - 1) = fill_symbol;

        // Define a variable for how many symbols are in a row
        int symbols_in_a_row = 0;

        // Loops through each row element horizontally
        for(unsigned int j = 0; j < SIZE; ++j){
            if (board_.at(y-1).at(j) == fill_symbol){
                symbols_in_a_row++;
            } else {
                symbols_in_a_row = 0;
            }
            if (symbols_in_a_row == 3) {
                // Too many symbols in a horizontal row
                // Empties the coordinate where the symbol was added
                board_.at(y - 1).at(x - 1) = EMPTY;
                return true;
            }
        }

        // Reset variable back to 0 so we can loop through vertical rows
        symbols_in_a_row = 0;

        // Loops through each row element vertically
        for(unsigned int j = 0; j < SIZE; ++j){
            if (board_.at(j).at(x - 1) == fill_symbol){
                symbols_in_a_row++;
            } else {
                symbols_in_a_row = 0;
            }
            if (symbols_in_a_row == 3) {
                // Too many symbols in a vertical row.
                // Empties the coordinate where the symbol was added
                board_.at(y - 1).at(x - 1) = EMPTY;
                return true;
            }
        }
        return false; // No symbol had a row of 3, return false
}

bool GameBoard::is_coordinate_empty(int y, int x){
    if (board_.at(y - 1).at(x - 1) == EMPTY) {
        return true;
    } else {
        return false;
    }
}

void GameBoard::create_board_row(std::vector<Element_type> row){
    board_.push_back(row);
}
