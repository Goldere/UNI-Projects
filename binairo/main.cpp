#include "gameboard.hh"
#include <iostream>
#include <random>

using namespace std;

// Output messages
const string QUIT = "Quitting ...";
const string OUT_OF_BOARD = "Out of board";
const string INVALID_INPUT = "Invalid input";
const string CANT_ADD = "Can't add";
const string WIN = "You won!";

// Converts the given numeric string to the corresponding integer
// (by calling stoi) and returns the integer.
// If the given string is not numeric, returns zero.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Removes empty characters (such as ' ' etc.) from the given string.
// Returns true if the given string has exactly one non-empty character,
// which is either '0' or '1', otherwise returns false.
bool find_fill_symbol(string& str)
{
    string fill_str = "";
    for(unsigned int i = 0; i < str.size(); ++i)
    {
        if(not isspace(str.at(i)))
        {
            fill_str += str.at(i);
        }
    }
    str = fill_str;
    return (fill_str.size() == 1 and
           (fill_str.at(0) == '0' or fill_str.at(0) == '1'));
}

// Enables the user to play the game, i.e. by repeatedly asking an element
// to be added and its position, until the game is over.
void play_game(GameBoard& board)
{
    board.print();
    while(not board.is_game_over())
    {
        string x_str = "";
        string y_str = "";
        string rest_input = "";
        cout << "Enter two coordinates and a fill symbol, or q to quit: ";

        // Reading x coordinate as a string and checking if it was quit command
        cin >> x_str;
        if(x_str.at(0) == 'q' or x_str.at(0) == 'Q')
        {
            cout << QUIT << endl;
            return;
        }

        // Reading y coordinate
        cin >> y_str;

        // Changing coordinates from string to int and checking if they are
        // inside the board
        unsigned int x = stoi_with_check(x_str);
        unsigned int y = stoi_with_check(y_str);
        if(not (1 <= x and x <= SIZE and 1 <= y and y <= SIZE))
        {
            cout << OUT_OF_BOARD << endl;
            getline(cin, rest_input);
            continue;
        }

        // Reading the rest of the input line including fill symbol and
        // checking if the rest input consists exactly of one '0' or '1'
        getline(cin, rest_input);
        if(not find_fill_symbol(rest_input))
        {
            cout << INVALID_INPUT << endl;
            continue;
        }

        Element_type fill_symbol = EMPTY;

        if (rest_input == "1") {
            fill_symbol = ONE;
        } else if (rest_input == "0"){
            fill_symbol = ZERO;
        }

        // Checks if the chosen coordinate is already occupied
        if (!board.is_coordinate_empty(y, x)){
            std::cout << "Can't add" << std::endl;
        }

        // Checks if adding the symbol breaks any game rules
        else if (board.too_many_symbols_horizontally(y, fill_symbol) ||
                   board.too_many_symbols_vertically(x, fill_symbol)){
            std::cout << "Can't add" << std::endl;
        }

        // Checks if adding the symbol breaks any game rules
        else if (board.too_many_symbols_consecutively(y, x, fill_symbol)){
            std::cout << "Can't add" << std::endl;

        }

        // Add the symbol to the board and print the changed game board
        else {
            board.add_fill_symbol(y, x, fill_symbol);
            // If the given symbol was possible to add, print the changed gameboard
            board.print();
        }
    }

    // If the game ends up to a totally filled gameboard, the player won
    cout << WIN << endl;
}
// Gives the user a possibility to select a filling way.
// Returns true if filling succeeded, otherwise returns false.
bool select_start(GameBoard& board)
{
    string choice = "";
    cout << "Select start (R for random, I for input): ";
    getline(cin, choice);
    if(choice != "R" and choice != "r" and choice != "I" and choice != "i")
    {
        return false;
    }
    else if(choice == "R" or choice == "r")
    {
        string seed_string = "";
        cout << "Enter a seed value: ";
        getline(cin, seed_string);

        int seed_value = std::stoi(seed_string);

        // Defines random number generator
        default_random_engine gen(seed_value);
        uniform_int_distribution<int> distr(0, DISTR_UPPER_BOUND);

        // Checks if the seed is a bad seed
        if (board.is_bad_seed(seed_value) == false) {
            int random_number = distr(gen);

            // Fill the board with random symbols
            for( int y = 0; y < SIZE; ++y ) {
                std::vector< Element_type > row;

                for( int x = 0; x < SIZE; ++x ) {
                    if (random_number == 0){
                       row.push_back(ZERO);
                    } else if (random_number == 1) {
                        row.push_back(ONE);
                    } else {
                        row.push_back(EMPTY);
                    }
                    random_number = distr(gen);
                }
                board.create_board_row(row);
            }
            return true; // Random start successful
        } else {
            return false; // Bad seed, exit function
        }
    }
    else if(choice == "I" or choice == "i"){
        string input = "";
        cout << "Input: ";
        getline(cin, input);

        std::vector< Element_type > row;

        // Checks whether input length is right size
        if (input.length() != 38){
            std::cout << "Wrong size of input" << std::endl;
            return false; // Invalid input size, exit function
        }

        // Checks if there are any other characters besides " ", 0 & 1
        for (int i = 1; i < 37; ++i) {
            if (input[i] != ' ' && input[i] != '0' && input[i] != '1'){
                std::cout << "Wrong character" << std::endl;
                return false; // Invalid character, exit function
            }
        }

        // Creates rows for the board
        for (int i = 1; i < 37; ++i) {
            if (input[i] == '1') {
                row.push_back(ONE);
            } else if (input[i] == '0'){
                row.push_back(ZERO);
            } else {
                row.push_back(EMPTY);
            }

            //If row has 6 elements, adds the row to the gameboard.
            if (i % 6 == 0) {          
                board.create_board_row(row);
                row.clear();
            }
        }
    }
    return true; // Input start successful
}

// Short and simple main function.
int main()
{
    GameBoard board;
    while(not select_start(board)); // no code to be repeated

    play_game(board);
    return 0;
}
