#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>

/*
 * Class Player
 * ----------------------
 * COMP.CS.110 SPRING 2021
 * ---------------
 * Class for presenting each player playing the game.
**/

class Player
{
public:

    // Constructor: creates a player with the given name.
    Player(const std::string& name);

    // Returns the name of the player.
    std::string getName() const;

    // Returns the number of pairs collected by the player so far.
    unsigned int numberOfPairs() const;

    // Prints the game status of the player: name and collected pairs so far.
    // Not actually used here, but could be useful.
    void print() const;

    // Add score to player. Adds 2 points to player.
    void addScore();


private:

    std::string player_name_;
    int     score_;
};

#endif // PLAYER_HH
