/* Binairo
 *
 * Description:
 * This program is designed to implement the Binairo game,
 * which provides the user with a 6x6 grid(this can be changed, by
 * changing the variable NUMBER_OF_SYMBOLS in gameboard.hh file.
 * The grid allows the placement of circles, triangles, or leaving a slot empty
 * The objective is to fill the empty slots according to the following rules:
 *
 * Each row and column can have a maximum of three identical numbers.
 * Each row and column can have a maximum of two consecutive identical numbers.
 *
 * The program offers two options to start the game:
 *
 * Random Fill: The user can fill the grid with randomly
 * generated symbols. If the user chooses this option, they are also
 * required to provide a seed for the random number generator.
 *
 * User's Choice: The user can input their own 36 symbols into the grid.
 * Before accepting the input, the program checks whether the symbols
 * are valid or not.
 *
 * As the game progresses, the user can input symbol and its coordinates on
 * the game board each round. The game ends in a player's victory if the
 * game board is filled according to the rules. The program allows additions
 * that violate the rules, but they are shown with a red background. If the
 * game rules are ever violated, the game is automatically lost but can be
 * played until the board is filled.
 *
 * The program checks the validity of inputs.
 * Input must follow these rules:
 *
 * The additional symbol must be C(circle) or T(triangle).
 * Coordinates must be within the grid.
 * The indicated coordinate slot must be empty.
 *
 * */

#include "mainwindow.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
