

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QTimer>
#include <QIcon>
#include <iostream>
#include <string>
#include <vector>
#include "gameboard.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer* timer;

private slots:

    // Starts the game after checking the seed or input was valid,
    // enabling all the buttons that are used during the game and
    // disabling the rest. Also setups the game timer and changes the
    // game background color. Creates the game board by calling
    // setup_gameboard() & update_gameboard_labels() functions.
    void on_playButton_clicked();

    // Empties the gameboard & calls for on_playButton_clicked()
    // function, starting the game all over again.
    void on_resetButton_clicked();

    // Activates when text is changed within the y_coordinate label.
    // Makes sure the given coordinate is valid and updates the
    // y_coordinate_ variable with it.
    void y_coordinate_textChanged(const QString &input);

    // Activates when text is changed within the x_coordinate label.
    // Makes sure the given coordinate is valid and updates the
    // x_coordinate variable with it.
    void x_coordinate_textChanged(const QString &input);

    // Activates when text is changed within the symbol label.
    // Makes sure the given symbol is valid and updates the
    // fill_symbol_ variable with it.
    void symbol_textChanged(const QString &input);

    // Activates when text is changed within the input label.
    // Makes sure the given symbol is valid and updates the
    // input_ variable with it.
    void input_textChanged(const QString &input);

    // Activates when text is changed within the seed label.
    // Makes sure the given seed is valid and updates the
    // seed_ variable with it.
    void seed_textChanged(const QString &input);

    // Activates when insertButton is clicked.
    // Adds symbol to the game board if the y and x coordinates are within
    // the game board and the label isn't already occupied by another symbol.
    // Checks also if the game ended.
    void on_insertButton_clicked();

    // Pauses the game, disabling all of the interface elements that are
    // used during the game. If game is paused, continues the game by
    // by enabling these elements.
    void on_pauseButton_clicked();

    // Closes the mainwindow.
    void on_closeButton_clicked();

    // Adds one second to the game timer and updates the game timer.
    // If 10 seconds has passed, decreases the points by 2 and updates
    // points display.
    void update_timer();

private:
    Ui::MainWindow *ui;

    GameBoard game_board;

    QGridLayout *grid_layout;

    // Vector to store labels
    QVector<QVector<QLabel*>> labels;

    // Coordinates used to insert symbols to the game board.
    int x_coordinate_= 0;
    int y_coordinate_= 0;

    // Bool value to check whether the game board is already setted up.
    bool game_board_set_ = false;

    // Bool value to check whether the game is paused.
    bool game_paused_ = false;

    // Gameboard layout options.
    int seed_ = -1;
    std::string input_ = "";

    // Game time variables.
    int minutes_ = 0;
    int seconds_ = 0;

    // Game points
    int game_points_ = 100;

    // Symbol added to the game board.
    char fill_symbol_ = ' ';

    // Function to setup the UI with labels
    void setup_gameboard();

    // Function to update labels based on game board state
    void update_gameboard_labels();

};
#endif // MAINWINDOW_HH
