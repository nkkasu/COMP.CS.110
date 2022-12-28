#include "card.hh"
#include <iostream>

Card::Card(const char c):
    letter_(c)
{

}

void Card::setLetter(const char c)
{
    letter_ = c;
}

char Card::getLetter() const
{
    return letter_;
}
