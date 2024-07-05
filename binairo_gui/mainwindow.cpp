#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setInterval(1000);

    grid_layout = new QGridLayout(ui->centralwidget);

    connect(ui->coordinate_y, &QLineEdit::textChanged,
            this, &MainWindow::y_coordinate_textChanged);

    connect(ui->coordinate_x, &QLineEdit::textChanged,
            this, &MainWindow::x_coordinate_textChanged);

    connect(ui->symbol, &QLineEdit::textChanged,
            this, &MainWindow::symbol_textChanged);

    connect(ui->input, &QLineEdit::textChanged,
            this, &MainWindow::input_textChanged);

    connect(ui->seed, &QLineEdit::textChanged,
            this, &MainWindow::seed_textChanged);

    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer);

    ui->centralwidget->setStyleSheet("background-color: lightgray;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked(){
    // Check if we are using seed or input

    if (input_.size() > 0){
        // Make sure input_ consist of C or T characters.
        for (char c : input_) {
            if (c != 'C' and c != 'T' and c != ' '){
                ui->gameStatusLabel->setText("Bad Input!");
                return;
            }
        }

        // Check whether the input_ size is allowed
        if (input_.size() != (SIZE * SIZE + 2)){
            ui->gameStatusLabel->setText("Incorrect Input!");
            return;
        }
        else {
            game_board.fill_from_input(input_);
        }
    }
    else if (seed_ >= 0) {
        // Check whether seed_ is valid.
        if (game_board.fill_randomly(seed_) == false) {
            ui->gameStatusLabel->setText("Bad Seed!");
            return;
        } else {
            game_board.fill_randomly(seed_);
        }
    }

    else {
        ui->gameStatusLabel->setText("Bad Seed or Input!");
        return;
    }

    // Reset background color
    ui->centralwidget->setStyleSheet("background-color: lightgray;");

    // Reset timer
    seconds_ = 0;
    minutes_ = 0;

    // Reset points
    game_points_ = 100;
    ui->gamePoints->display(game_points_);

    // Enable buttons used during game
    ui->coordinate_y->setEnabled(true);
    ui->coordinate_x->setEnabled(true);
    ui->insertButton->setEnabled(true);
    ui->symbol->setEnabled(true);
    ui->resetButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);

    // Disable everything else
    ui->playButton->setEnabled(false);
    ui->input->setEnabled(false);
    ui->seed->setEnabled(false);

    // Setup the game board UI
    if (!game_board_set_){
        setup_gameboard();
    }

    // Update labels based on game board state
    update_gameboard_labels();

    // Align the grid layout within the central widget
    grid_layout->setAlignment(Qt::AlignTop); // Adjust alignment as needed

    // Set minimum width for columns
    for (unsigned int j = 0; j < SIZE; ++j) {
        grid_layout->setColumnMinimumWidth(j + 1, 30);
    }

    // Set minimum height for rows
    for (unsigned int i = 0; i < SIZE; ++i) {
        grid_layout->setRowMinimumHeight(i + 1, 30);
    }

    // Start timer
    timer->start(1000);
}


void MainWindow::on_resetButton_clicked(){
    ui->gameStatusLabel->setText("");

    // Clear board
    game_board.clear_board();

    // Create a new board and restart the game.
    on_playButton_clicked();
}

void MainWindow::on_insertButton_clicked(){
    if (x_coordinate_ > 0 and y_coordinate_ > 0) {
        if (game_board.add_symbol(x_coordinate_ - 1,
                                  y_coordinate_ - 1, fill_symbol_) == true) {
            ui->gameStatusLabel->setText("Symbol added!");
        } else {
            ui->gameStatusLabel->setText("Bad Position!");
        }
    } else {
        ui->gameStatusLabel->setText("Can't add!");
    }

    update_gameboard_labels();


    if (game_board.is_game_over()){
        // Game ended

        // Disable buttons used during game
        ui->coordinate_y->setEnabled(false);
        ui->coordinate_x->setEnabled(false);
        ui->insertButton->setEnabled(false);
        ui->symbol->setEnabled(false);
        ui->resetButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);

        // Enable reset button & stop timer
        ui->resetButton->setEnabled(true);
        timer->stop();

        // Check whether the player won or lost the game.
        if (game_board.is_game_won()){
            ui->gameStatusLabel->setText("You Won!");
            ui->centralwidget->setStyleSheet("background-color: lightgreen;");
        } else {
            ui->gameStatusLabel->setText("You Lost...");
            ui->centralwidget->setStyleSheet("background-color: firebrick;");
        }

    }
}

void MainWindow::on_pauseButton_clicked()
{
    if (game_paused_ == false) {
        // Game pause enabled, disable buttons.
        game_paused_ = true;
        ui->coordinate_y->setEnabled(false);
        ui->coordinate_x->setEnabled(false);
        ui->insertButton->setEnabled(false);
        ui->symbol->setEnabled(false);
        ui->resetButton->setEnabled(false);
        timer->stop();
        ui->gameStatusLabel->setText("Game Paused!");
        ui->pauseButton->setText("Continue");
    }

    else if (game_paused_ == true) {
        // Game pause disabled, enable buttons.
        game_paused_ = false;
        ui->coordinate_y->setEnabled(true);
        ui->coordinate_x->setEnabled(true);
        ui->insertButton->setEnabled(true);
        ui->symbol->setEnabled(true);
        ui->resetButton->setEnabled(true);
        ui->gameStatusLabel->setText("");
        ui->pauseButton->setText("Pause");
        timer->start(1000);
    }
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::update_timer()
{
    seconds_++;

    // If ten seconds have passed, minus two points.
    if (seconds_ % 10 == 0) {

        // Make sure that the player can't get negative amount of points.
        if (game_points_ != 0) {
            game_points_ -= 2;
        }

        ui->gamePoints->display(game_points_);
    }

    // If the seconds reach 60, reset to 0 and increment the minutes
    if (seconds_ == 60) {
         seconds_ = 0;
         minutes_++;
    }

    // Display the time in the format "mm:ss"
    ui->gameTimer->display(QString("%1:%2")
                      .arg(minutes_, 2, 10, QChar('0'))
                      .arg(seconds_, 2, 10, QChar('0')));
}


void MainWindow::y_coordinate_textChanged(const QString &input)
{
    bool conversion_ok = false;
    int integer_value = input.toInt(&conversion_ok);

    // Make sure the y coordinate is allowed.
    if (!conversion_ok or integer_value <= 0 or integer_value > SIZE) {
        y_coordinate_ = 0;
        return;
    }

    y_coordinate_ = integer_value;
}

void MainWindow::x_coordinate_textChanged(const QString &input)
{
    bool conversion_ok = false;
    int integer_value = input.toInt(&conversion_ok);

    // Make sure the x coordinate is allowed.
    if (!conversion_ok or integer_value <= 0 or integer_value > SIZE) {
        x_coordinate_ = 0;
        return;
    }

    x_coordinate_ = integer_value;
}

void MainWindow::symbol_textChanged(const QString &input)
{
    std::string input_string = input.toStdString();

    fill_symbol_ = ' ';

    if (input_string.size() == 1) {
        fill_symbol_ = input_string[0];
    }
}

void MainWindow::input_textChanged(const QString &input)
{
    if (input != ""){
        ui->seed->setText("");
        seed_ = -1;
        input_ = input.toStdString();
    }
}

void MainWindow::seed_textChanged(const QString &input)
{
    bool conversion_ok = false;
    int integer_value = input.toInt(&conversion_ok);

    if (!conversion_ok or integer_value <= 0) {
        seed_ = -1;
        return;
    }

    if (input != ""){
        input_ = "";
        ui->input->setText("");
        seed_ = input.toInt();
    }
}

void MainWindow::setup_gameboard()
{
    grid_layout->setSpacing(10); // Set spacing between widgets

    game_board_set_ = true;

    // Add column numbers at the top
    for (unsigned int j = 0; j < SIZE; ++j) {

         // Column numbers start from 1
        QLabel *columnLabel = new QLabel(QString::number(j + 1));

        columnLabel->setAlignment(Qt::AlignCenter);

         // Add to the first row (row 0), starting from column 1
        grid_layout->addWidget(columnLabel, 0, j + 1);
    }

    // Add row numbers at the beginning of each row.
    // and labels for the game board.
    for (unsigned int i = 0; i < SIZE; ++i) {
        // Add row number.
        // Row numbers start from 1.
        QLabel *rowLabel = new QLabel(QString::number(i + 1));
        rowLabel->setAlignment(Qt::AlignCenter);

        // Add to each row (starting from row 1), first column.
        grid_layout->addWidget(rowLabel, i + 1, 0);


        QVector<QLabel*> row_labels;

        // Add labels for the game board.
        for (unsigned int j = 0; j < SIZE; ++j) {
            QLabel *label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet("QLabel { background-color : white; }");

            grid_layout->addWidget(label, i + 1, j + 1);

            row_labels.push_back(label);
        }
        labels.push_back(row_labels);
    }
}

void MainWindow::update_gameboard_labels()
{
    // Define game symbols
    QIcon circle(":/images/circle.png");
    QIcon triangle(":/images/triangle.png");

    for (unsigned int i = 0; i < SIZE; ++i) {
        for (unsigned int j = 0; j < SIZE; ++j) {

            Element_type element_type = game_board.board_[i][j];

            QLabel *label = labels[i][j];

            QPixmap current_pixmap = label->pixmap();

            label->setStyleSheet("QLabel { background-color : white; }");

            // Updating labels
            switch(element_type) {
                case ZERO:
                    label->setPixmap(triangle.pixmap(QSize(32, 32)));
                    break;
                case ONE:
                    label->setPixmap(circle.pixmap(QSize(32, 32)));
                    break;
                case EMPTY:
                    label->clear();
                    break;
                case INCORRECT:            
                    if (current_pixmap.isNull()) {
                        if (fill_symbol_ == 'C'){
                            label->setPixmap(circle.pixmap(QSize(32, 32)));
                        } else if (fill_symbol_ == 'T'){
                            label->setPixmap(triangle.pixmap(QSize(32, 32)));
                        }
                     }
                    label->setStyleSheet("QLabel { background-color : red; }");
                    break;
            }
        }
    }
}

