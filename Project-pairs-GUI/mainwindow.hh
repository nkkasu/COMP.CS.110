#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "player.hh"
#include "card.hh"
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <vector>
#include <iostream>
#include <random>
#include <QTimer>


/*
 * Mainwindow for Pairs gui program.
 * ----------------------
 * COMP.CS.110 SPRING 2021
 * ---------------
 * Mainwindow for main functions of pairs gui program. Game logic,
 * Data structures etc.
**/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Turn cards if 2 are not turned already. If 2 cards
    // are turned, doesn't respond to click. If the turned
    // card is a second one, calls other functions to
    // check for pair.
    void turnCard();

    // Edit name in attribute player_ when LineEdit editing
    // is finished,
    void editName();

    // Add player into structures when add-button is clicked.
    void playerAdd();

    // Hide all cards in a game board.
    void hideCards();

    // Start game. Change states of ui buttons and labels.
    void runGame();

    // Switch player who is in turn. Update text labels
    // (playerInTurn etc.)
    void changeTurn();

    // Remove pair from game board. Add it to player score.
    void removePair();

    // Update timer every interval of 1000ms (1 second).
    void onTimerTimeout();

private:
    Ui::MainWindow *ui;

    // Maximum value 30, although 26 not recommended because of
    // how cards are placed based on factors.
    // Also has to be even number.
    const int CARDCOUNT = 24;

    // Timer to keep track of time.
    QTimer* timer;

    // Keeping track whether 0, 1 or 2 turned cards.
    int cardsTurned_;

    // Keeping track of cards currently on the table.
    unsigned int cardsOnTable_;

    // String used to randomize letters in a button.
    const std::string LETTERS = "AABBCCDDEEFFGGHHJJIIKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";

    // Name of player who is trying to be added.
    std::string playerName_;

    // For QTimer, keeping track of seconds
    int seconds_;

    // For QTimer, keeping track of minutes.
    int minutes_;

    // Delay for singleshot. 1 second (1000ms)
    int DELAY = 1000;

    // Random number generator for cards
    std::random_device rndDev_;

    size_t turnNumber_;
    // Each button and it's info is stored on map.
    std::map<QPushButton*, Card> buttons_;

    // Currently open cards are stored in this vector (size should be max. 2)
    std::vector<std::pair<QPushButton*, Card>> openCards_;

    std::vector<Player> players_;
    // Calculating factors to place buttons in a desirable grid.
    void calculateFactors(unsigned int& smallerFact, unsigned int& biggerFact);

    // Initialize all buttons as well as background behind buttons.
    void initButtons();

    // Disable all cards to avoid from being clicked.
    void disableCards();

    // Function to connect slots that dont need to be connected in a loop.
    void connectSlots();

    // Enable all cards
    void enableCards();

    // Check for pairs.
    void checkForPair();

    // Add pairs to current player in turn.
    void addPoints();

    // For things to do when game ends.
    void endGame();

    // Function to display win or tie.
    void displayWinOrTie();

    // Print winner of game, if there is one.
    void printWinner(int& tieOfHowMany, Player& winner);
};
#endif // MAINWINDOW_HH
