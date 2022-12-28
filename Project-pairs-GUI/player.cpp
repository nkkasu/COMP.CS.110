#include "player.hh"
#include <iostream>

Player::Player(const std::string &name):
player_name_(name),
score_(0)
{
}

std::string Player::getName() const
{
    return player_name_;
}

unsigned int Player::numberOfPairs() const
{
    return score_ / 2;
}

void Player::print() const
{
    std::cout << "*** " << player_name_ << " has " << score_ / 2
              << " pair(s)." << std::endl;
}

void Player::addScore()
{
    score_ += 2;
}

