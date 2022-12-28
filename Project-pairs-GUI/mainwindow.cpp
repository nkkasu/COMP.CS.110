#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "player.hh"
#include "card.hh"
#include <algorithm>
#include <random>
#include <QGridLayout>
#include <QPushButton>
#include <QChar>
#include <QTextBrowser>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      timer(new QTimer(this)),
      cardsTurned_(0),
      seconds_(0),
      minutes_(0),
      rndDev_(),
      turnNumber_(0)

{
    ui->setupUi(this);
    // Init buttons of gaming board.
    initButtons();

    // Disable cards, game hasn't started yet.
    disableCards();

    // There are CONST CARDCOUNT cards on table in
    // beginning.
    cardsOnTable_ = CARDCOUNT;
    ui->timerLCD->display("00:00");
    ui->startGameButton->setDisabled(true);
    this->setWindowTitle("Pairs game");
    this->setFixedSize(800, 500);
    connectSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::turnCard()
{
    // If 2 cards are turned, we don't want to do anything.
    if (cardsTurned_ == 2)
    {
        return;
    }
    // 2 cards are not turned, so we can turn a card. Add to count.
    ++cardsTurned_;

    // Get button pressed.
    QPushButton* pressedButton = static_cast<QPushButton*>(sender());
    pressedButton->setDisabled(true);

    // Remove image, back is turned.
    pressedButton->setIcon(QIcon());
    pressedButton->setStyleSheet("QPushButton { background-color : white; color : black; }");

    // Make a pair of pushbutton, card for each card.
    Card card = buttons_.at(pressedButton).getLetter();
    QChar c = card.getLetter();
    std::pair<QPushButton*, Card> cardPair = {pressedButton, card};

    // Add card just opened to openCards-vector.
    openCards_.push_back(cardPair);
    pressedButton->setText(c);
    // 2 cards opened, check for pair.
    if (openCards_.size() == 2)
    {
        checkForPair();
    }
}

void MainWindow::editName()
{
    playerName_ = ui->lineEdit->text().toStdString();
}

void MainWindow::playerAdd()
{
    // Make a new Player object.
    Player new_player(playerName_);
    // Empty lineEdit.
    ui->lineEdit->setText("");
    // Add to a vector.
    players_.push_back(new_player);
    ui->startGameButton->setEnabled(true);
}

void MainWindow::hideCards()
{
    // We can only hide cards if 2 cards are turned.
    if (cardsTurned_ == 2)
    {
    // Go through each button and remove text and add image back.
    for (std::pair<QPushButton*, Card> button : buttons_)
    {
    button.first->setText("");
    button.first->setStyleSheet("QPushButton { background-color : rgb(77, 77, 77); color : black; }");
    button.first->setIcon(QIcon(":/backside.png"));
    }
    // Cards are hidden, so 0 are turned.
    cardsTurned_ = 0;
    openCards_.clear();
    // Cards can be clicked again.
    enableCards();
    }
}

void MainWindow::runGame()
{
    // Start the timer
    timer->start(1000);

    // start button cannot be pressed and players cannot be added
    // once game starts.
    ui->startGameButton->setDisabled(true);
    ui->lineEdit->setDisabled(true);
    ui->addButton->setDisabled(true);
    // Game starts, card can be pressed.
    enableCards();
    // set visible count for pairs and turn.
    ui->pairCountLabel->setVisible(true);
    ui->inTurnLabel->setVisible(true);
    QString playerName = QString::fromStdString(players_.at(0).getName());
    ui->playerInTurn->setText(playerName);
    unsigned int points = players_.at(turnNumber_).numberOfPairs();
    std::string pts_str = std::to_string(points);
    QString pts = QString::fromStdString(pts_str);
    ui->pairsLabel->setText(pts);
}

void MainWindow::changeTurn()
{
    // Start a new round
    if (turnNumber_ == players_.size() - 1)
    {
        turnNumber_ = 0;
    }
    else
    {
        // Change turn
        ++turnNumber_;
    }
    // Update label of player in turn.
    QString playerName = QString::fromStdString(players_.at(turnNumber_).getName());
    ui->playerInTurn->setText(playerName);

    // Update points label to new player in turn.
    unsigned int points = players_.at(turnNumber_).numberOfPairs();
    std::string ptsStr = std::to_string(points);
    QString qPts = QString::fromStdString(ptsStr);
    ui->pairsLabel->setText(qPts);
}

void MainWindow::removePair()
{
    // Remove pair from table.
    std::pair<QPushButton*, Card> cardOne = openCards_.at(0);
    std::pair<QPushButton*, Card> cardTwo = openCards_.at(1);
    cardOne.first->hide();
    cardTwo.first->hide();
}

void MainWindow::onTimerTimeout()
{
    // Reset seconds in a timer if new minute is about to start.
    if (seconds_ == 59)
    {
        minutes_ += 1;
        seconds_ = 0;
    }
    else
    {
        // add seconds to a timer.
        ++seconds_;
    }
    std::string secondsStr = std::to_string(seconds_);
    std::string minutesStr = std::to_string(minutes_);
    std::string time = "";
    // Convert string to 00:00 format instead of 0:0
    if (seconds_ < 10 && minutes_ < 10)
    {
        time = "0" + minutesStr + ":" + "0" + secondsStr;
    }
    else if (seconds_ < 10)
    {
        time = minutesStr + ":" + "0" + secondsStr;
    }
    else if (minutes_ < 10)
    {
        time = "0" + minutesStr + ":" + secondsStr;
    }
    else
    {
        time = minutesStr + ":" + secondsStr;
    }
    // Show time.
    QString qTime = QString::fromStdString(time);
    ui->timerLCD->display(qTime);
}

void MainWindow::calculateFactors(unsigned int &smaller_factor, unsigned int &bigger_factor)
{
    // Calculate factors so cards are placed in a good formatting.
    unsigned int product = 0;
    product = CARDCOUNT;
    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

void MainWindow::initButtons()
{
    // Index to add letters to a button in a randomized order.
    int ind = 0;

    // Getting factors to place buttons in a good order.
    unsigned int rows = 0;
    unsigned int columns = 0;
    calculateFactors(rows, columns);

    // Gridlayout placed inside this widget.
    QWidget* gameBoard = new QWidget(this);
    gameBoard->setGeometry(0, 0, 500, 500);
    gameBoard->setFixedSize(500, 500);
    gameBoard->setStyleSheet("background-color: rgb(196, 155, 108)");

    // Gridlayout for buttons.
    QGridLayout* gameGridLayout = new QGridLayout(gameBoard);
    std::string letters = LETTERS.substr(0, CARDCOUNT);

    // Call random number generator to shuffle cards.
    shuffle(letters.begin(), letters.end(), std::mt19937(rndDev_()));

    // Create buttons in a grid.
    for (unsigned int i = 0; i < rows; ++i)
    {
        for (unsigned int j = 0; j < columns; ++j)
        {
            char letter = letters.at(ind);
            Card new_card(letter);
            QPushButton* pushbutton = new QPushButton(this);
            pushbutton->setIcon(QIcon(":/backside.png"));
            pushbutton->setIconSize(QSize(200, 100));
            pushbutton->setFixedWidth(45);
            pushbutton->setFixedHeight(70);

            // Size needs to be retained, so grid size doesnt change when buttons are hidden.
            auto sp_retain = pushbutton->sizePolicy();
            sp_retain.setRetainSizeWhenHidden(true);
            pushbutton->setSizePolicy(sp_retain);
            // Change button color and add it to a map. Connect button press.
            pushbutton->setStyleSheet("QPushButton { background-color : rgb(77, 77, 77); color : black; }");
            gameGridLayout->addWidget(pushbutton, i, j, 1, 1);
            buttons_.insert({pushbutton, new_card});
            connect(pushbutton, &QPushButton::clicked, this, &MainWindow::turnCard);
            ++ind;
        }
    }
}

void MainWindow::disableCards()
{
    for (auto button : buttons_)
    {
        button.first->setDisabled(true);
    }
}

void MainWindow::connectSlots()
{
    // Connect all slots that don't need to be connected in a loop.
    connect(ui->lineEdit, &QLineEdit::editingFinished, this, &MainWindow::editName);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::playerAdd);
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::runGame);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
}

void MainWindow::enableCards()
{
    for (std::pair<QPushButton*, Card> button : buttons_)
    {
    button.first->setEnabled(true);
    }
}

void MainWindow::checkForPair()
{
    // Get both cards that are turned.
    std::pair<QPushButton*, Card> cardOne = openCards_.at(0);
    std::pair<QPushButton*, Card> cardTwo = openCards_.at(1);

    // Compare if both of the opened cards letters are the same.
    if (cardOne.second.getLetter() == cardTwo.second.getLetter())
    {
        // Remove cards.
        cardsOnTable_ -= 2;
        QTimer::singleShot(DELAY, this, SLOT(removePair()));
        // Add points to current player.
        addPoints();
        // Get pairs of player.
        unsigned int points = players_.at(turnNumber_).numberOfPairs();
        std::string ptsStr = std::to_string(points);
        // QString of player pairs. Set text to label.
        QString qPtsStr = QString::fromStdString(ptsStr);
        ui->pairsLabel->setText(qPtsStr);
        // If pair found and no cards left on table, game is over.
        if (cardsOnTable_ == 0)
        {
            endGame();
        }
    }
    else
    {
        // Game is not over. Change turn in a delay.
        QTimer::singleShot(DELAY, this, SLOT(changeTurn()));
    }
    // Hide cards in a delay.
    QTimer::singleShot(DELAY, this, SLOT(hideCards()));
}

void MainWindow::addPoints()
{
    players_.at(turnNumber_).addScore();
}
void MainWindow::endGame()
{
    timer->stop();
    ui->inTurnLabel->hide();
    ui->playerInTurn->hide();
    ui->pairsLabel->hide();
    ui->pairCountLabel->hide();
    displayWinOrTie();
}

void MainWindow::displayWinOrTie()
{
    // First get (one) winner of match.
    int tieOfHowMany = 0;
    Player winner = players_.at(0);
    for (Player playerName : players_)
    {
        if (playerName.numberOfPairs() > winner.numberOfPairs())
        {
            winner = playerName;
        }
    }
    // Check if game is actually a tie between x players.
    for (Player playerName : players_)
    {
        if (winner.numberOfPairs() == playerName.numberOfPairs())
        {
            tieOfHowMany += 1;
        }
    }
    // Print winner.
    printWinner(tieOfHowMany, winner);
}

void MainWindow::printWinner(int& tieOfHowMany, Player& winner)
{
    // If tie number is higher than 1, game is a tie. Display
    // tie of how many and how many pairs.
    if (tieOfHowMany > 1)
    {
        std::string tieOfStr = std::to_string(tieOfHowMany);
        std::string temp = "Game over! Tie of " + tieOfStr + " players.";
        QString qTieTxt = QString::fromStdString(temp);

        ui->winnerLabel->setText(qTieTxt);
    }
    else
    {
        // Game is not a tie. Display winner.
        unsigned int pairs = winner.numberOfPairs();
        std::string ptsStr = std::to_string(pairs);
        QString winnerPairs = QString::fromStdString(ptsStr);
        QString winnerName = QString::fromStdString(winner.getName());
        QString qWinTxt = "Game over! " + winnerName + " won with " + winnerPairs + " pairs!";
        ui->winnerLabel->setText(qWinTxt);
    }
}
