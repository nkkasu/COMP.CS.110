#ifndef CARD_HH
#define CARD_HH

/*
 * Class Card
 * ----------------------
 * COMP.CS.110 SPRING 2021
 * ---------------
 * Class for presenting each Card information of each button.
**/

class Card
{
public:

    // Constructor: creates a hidden card with the given character.
    Card(const char c);

    // Obvious setter and getter methods.
    void setLetter(const char c);
    char getLetter() const;

private:
    char letter_;
};

#endif // CARD_HH
