#include "player.hh"
#include <iostream>


Player::Player(const std::string &name):
player_name_(name),
score_(0)
{
}

std::string Player::get_name() const
{
    return player_name_;
}

unsigned int Player::number_of_pairs() const
{
    return score_ / 2;
}

void Player::add_card(Card &card)
{
    card.set_visibility(EMPTY);
    score_ += 1;
}

void Player::print() const
{
    std::cout << "*** " << player_name_ << " has " << score_ / 2
              << " pair(s)." << std::endl;
}
