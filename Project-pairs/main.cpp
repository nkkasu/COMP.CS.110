/* A pairs game
 *
 * Description:
 * Program executes a pairs (memory) game. Game has a varying amount of cards
 * and players. In the beginning of the game player is asks for a seed value,
 * since all cards are drawn randomly on the game board.
 * Game then asks how many players are going to play the game, and asks
 * to list the players. Every round player in turn gives coordinates of
 * two cards (x1, y1), (x2, y2). After coordinates are given, game checks
 * whether coordinates are valid, and then turns the cards around. Cards must
 * be found in the game board, as well as player can't choose two of the same
 * coordinate. Game checks whether cards are a pair or not. If cards were a pair,
 * player is given more score and cards are removed from the game board player
 * can also continue his turn and try finding more pairs. If cards were not a pair,
 * cards are turned back hidden. Player doesn't get any points and turn is given
 * to the next player. Cards are depicted as letters beginning from A to as far
 * as there are cards. When printing the game board, visible card is shown as
 * #, whereas hidden one as dot (.). Game ends when all pairs have been found,
 * and the game board is empty. The program tells who has/have won, as well as most
 * pairs collected.
 *
**/


#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

const string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const string INPUT_SEED = "Enter a seed value: ";
const string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const string INPUT_CARDS = "Enter two cards (x1, y1, x2, y2), or q to quit: ";
const string INVALID_CARD = "Invalid card.";
const string FOUND = "Pairs found.";
const string NOT_FOUND = "Pairs not found.";
const string GIVING_UP = "Why on earth you are giving up the game?";
const string GAME_OVER = "Game over!";

using Game_row_type = vector<Card>;
using Game_board_type = vector<vector<Card>>;

// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0
// (which leads to an invalid card later).
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Fills the game board, the size of which is rows * columns, with empty cards.
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Finds the next free position in the game board (g_board), starting from the
// given position start and continuing from the beginning if needed.
// (Called only by the function init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }

    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }

    // You should never reach this
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void init_with_cards(Game_board_type& g_board, int seed)
{

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);

    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);

    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {

        // Adding two identical cards (pairs) in the game board
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}

// Prints a line consisting of the given character c.
// The length of the line is given in the parameter line_length.
// (Called only by the function print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Prints a variable-length game board with borders.
void print(const Game_board_type& g_board)
{
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    print_line_with_char('=', columns);
    cout << "|   | ";
    for(unsigned int i = 0; i < columns; ++i)
    {
        cout << i + 1 << " ";
    }
    cout << "|" << endl;
    print_line_with_char('-', columns);
    for(unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < columns; ++j)
        {
            g_board.at(i).at(j).print();
            cout << " ";
        }
        cout << "|" << endl;
    }
    print_line_with_char('=', columns);
}

// Asks the desired product from the user, and calculates the factors of
// the product such that the factor as near to each other as possible.
void ask_product_and_calculate_factors(unsigned int& smaller_factor, unsigned int& bigger_factor)
{
    unsigned int product = 0;
    while(not (product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);
        product = stoi_with_check(product_str);
    }

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

// Splits a string by separator, which is in this case ' ' (aka whitespace).
// returns a vector, which has string split by separator in different indexes.
vector <string> split(string text)
{
    // Character we use as a separator to split text.
    char sep = ' ';

    // Text we want to split into parts.
    string text_to_split = text;

    // Vector to which we put those splitted parts.
    vector<string> split_of_space = {};

    // Used to build the character between separator characters
    // This is the string we put into the vector when we meet a
    // separator character.
    string make_word = "";

    // Looping through all the characters of a string we want to split.
    for (string::size_type i = 0; i < text_to_split.size(); ++i)
    {
        char character = text_to_split.at(i);

        if (character == sep)
        {
            // We put the built string into a vector.
            split_of_space.push_back(make_word);
            // Now that the word is in the vector, we reset the string
            make_word = "";
            // If the index i is same as the last letter of string,
            // we add it to vector (string ends)
            if (text_to_split.size() - 1 == i)
            {
                split_of_space.push_back(make_word);
            }
        }
        else
        {
            // If character is not a separator, add it to substring
            // make_word, which is later added into a vector.
            make_word += character;
            // If we are at last character of the string to split,
            // We add it to vector.
            if (text_to_split.size() - 1 == i)
            {
                split_of_space.push_back(make_word);
            }
        }
    }
    return split_of_space;
}

// Vector prompting how many players are gonna play the game,
// and then putting those players into a vector.
vector<Player> initialize_players()
{
    string player_count = "";
    while (true)
    {
        // Prompt how many players are gonna play the game
        cout << INPUT_AMOUNT_OF_PLAYERS;
        getline(std::cin, player_count);
        // Check if user input is valid.
        // If input is valid, stop prompting and break out of the loop
        if (stoi_with_check(player_count) != 0)
        {
            break;
        }
    }
    // Player objects are put into a vector.
    vector<Player> players_in_vector = {};

    // Prompt user again to give player_count amount of player names.
    std::cout << "List " << player_count << " players: ";
    string input = "";
    getline(cin, input);

    // Now that user has given player names, put them into vector with split.
    vector<string> player_names = split(input);

    // Make Player objects, using player_names vector.
    for(auto name : player_names)
    {
        Player object(name);
        players_in_vector.push_back(object);
    }
    // Return vector containing Player objects.
    return players_in_vector;
}

// Check if player has given a proper coords using stoi_with_check().
// No negative coordinates, no alphabet characters etc.
// Returns true, if input is valid and vice versa.
bool is_input_proper(string playerinput)
{
    // Split x1, y1, x2, y2 with split function.
    vector <string> vec = split(playerinput);
    string x1 = vec.at(0);
    string y1 = vec.at(1);
    string x2 = vec.at(2);
    string y2 = vec.at(3);

    // Check if stoi_with_check() returns a 0.
    // If it does, input of player is bad. -> return false
    if ((stoi_with_check(x1) == 0) || (stoi_with_check(y1) == 0)
         || (stoi_with_check(x2) == 0) || (stoi_with_check(y2)) == 0)
    {
        cout << INVALID_CARD << endl;
        return false;
    }
    // If none of the checks return 0, input is good -> return true.
    return true;
}

// Checks if flipping cards is allowed.
// If flipping is allowed, returns true
// If flipping is not allowed, returns false.
bool is_flip_allowed(string x1, string y1, string x2, string y2,
                     Game_board_type& game_board, unsigned int y_bnd,
                     unsigned int x_bnd)
{
    // Convert coordinates in string form into unsigned int,
    // so we can compare coords to upper and lower bounds of game board.
    unsigned int x1_int = stoi(x1);
    unsigned int y1_int = stoi(y1);
    unsigned int x2_int = stoi(x2);
    unsigned int y2_int = stoi(y2);

    // Check if given x- and y-coords are bigger than the bounds cards are within.
    if ((x1_int > x_bnd) || (x2_int > x_bnd) || (y1_int > y_bnd) || (y2_int > y_bnd))
    {
        cout << INVALID_CARD << endl;
        return false;
    }
    // Check if player tries to flip same card.
    else if ((x1_int == x2_int) && (y1_int == y2_int))
    {
        cout << INVALID_CARD << endl;
        return false;
    // Check if card is already taken from the game board.
    }
    else if (game_board.at(y1_int - 1).at(x1_int - 1).get_visibility() == EMPTY)
    {
        cout << INVALID_CARD << endl;
        return false;
    }
    // Same as above
    else if (game_board.at(y2_int - 1).at(x2_int - 1).get_visibility() == EMPTY)
    {
        cout << INVALID_CARD << endl;
        return false;
    }
    // If none are true, flipping cards is allowed with coords user gave.
    // -> return true
    return true;
}

// Used to flip the cards at the coordinates (x1, y1) (x2, y2)
// Returns true if pair is found
// Returns false if pair is not found.
bool flip_and_modify_cards(string x1, string y1, string x2, string y2,
               Game_board_type& game_board, vector<Player>& player, int in_turn)
{
    // Convert string coords into unsigned int.
    unsigned int x1_int = stoi(x1);
    unsigned int y1_int = stoi(y1);
    unsigned int x2_int = stoi(x2);
    unsigned int y2_int = stoi(y2);

    // To make code cleaner
    Card& card_one = game_board.at(y1_int - 1).at(x1_int - 1);
    Card& card_two = game_board.at(y2_int - 1).at(x2_int - 1);
    // Turn chosen cards around, making letter of those cards visible.
    card_one.turn();
    card_two.turn();

    // Print the game board, showing the turned cards.
    print(game_board);

    // If the letter of the turned cards is same,
    // pairs are found -> if = true
    if (card_one.get_letter() == card_two.get_letter())
    {
        // Print that pairs are found.
        cout << FOUND << endl;

        // Remove both cards from the game board.
        card_one.remove_from_game_board();
        card_two.remove_from_game_board();

        // Add card to player's score who is in turn.
        player.at(in_turn).add_card(card_two);
        player.at(in_turn).add_card(card_one);

        // Print current score of each player.
        for (auto player_object : player)
        {
            player_object.print();
        }
        print(game_board);
        return true;
    }
    // If pairs are not found we get here.
    else
    {
    // Print that cards are not found.
    cout << NOT_FOUND << endl;

    // Set visibility of the cards back to hidden.
    card_one.turn();
    card_two.turn();
    // Print current score of each player.
    for (auto player_object : player)
    {
        player_object.print();
    }
    // Print game board once again after hiding the cards.
    print(game_board);

    // Return false, since pairs were not found.
    return false;
    }
}
// Check if game has ended. Print who won or if game ended in a tie.
// Print how many pairs winner(s) had.
// returns true if game ended.
// returns false if it did not.
bool check_and_print_game_status(unsigned int cards, vector<Player>& player)
{
    // Value needed for loop to check, if any of the players have higher
    // score.
    unsigned int biggest_number_of_pairs = player.at(0).number_of_pairs();

    // String needed to set winner.
    string winner = player.at(0).get_name();

    // How many players ended up in a tie.
    int tie_of_how_many = 0;

    // If there are 0 cards left on the table, we check winner.
    if (cards == 0)
    {
        // Declare that game is over.
        cout << GAME_OVER << endl;

        // Find a winner from player objects.
        for (auto p : player)
        {
            // If iterated has more pairs, he is the new leader
            // in points.
            if (p.number_of_pairs() > biggest_number_of_pairs)
            {
                // Thus we set him as current winner.
                // Biggest number of pairs is also that player's
                // pair count.
                biggest_number_of_pairs = p.number_of_pairs();
                winner = p.get_name();
            }
        }

        // Check for a tie.
        for (auto p : player)
        {
            // If somebody has same amount of pairs as the winner,
            // We add one more into tie counter.
            if (biggest_number_of_pairs == p.number_of_pairs())
            {
                tie_of_how_many += 1;
            }
        }

        // If tie_of_how_many is greater than 1, there are more than 1
        // winner. Print accordingly.
        if (tie_of_how_many > 1)
        {
            cout << "Tie of " << tie_of_how_many << " players " << "with "
                 << biggest_number_of_pairs << " pairs.";
            return true;
        }

        // If there isn't a tie, we end here.
        cout << winner << " has won with " << biggest_number_of_pairs << " pairs.";
        return true;
    }
    // Game hasn't ended
    return false;

}

// The most crucial function to run the game.
// Takes in vector containing of player objects,
// vector<vector<Card>> containing cards at different coordinates
// upper and lower bounds.
// End game if there is a clear winner, OR somebody surrendered.
// Prompt for right (x1, y1), (x2, y2) until player gives one.
// Take turns until game is over
void run_game(vector<Player>& player, Game_board_type& game_board,
              unsigned int y_bound, unsigned int x_bound)
{
    // Get amount of cards at the board before the game has begun.
    unsigned int cards = y_bound * x_bound;

    // How many players are playing the game. Needed for loop.
    int loop_players = static_cast<int>(player.size());

    bool game_is_running = true;
    bool player_has_turn = true;

    while (game_is_running)
    {
        // Players take turns.
        for (int i = 0; i < loop_players; ++i)
        {
            while(player_has_turn)
            {
                // Check if there are 0 cards on the table. If yes
                // print game winner and end game.
                if (check_and_print_game_status(cards, player))
                {
                    game_is_running = false;
                    return;
                }
                // Prompt input of coords for two cards.
                cout << player.at(i).get_name() << ": "<<  INPUT_CARDS;
                string input_str = "";
                getline(cin, input_str);

                // If player chooses to give up, end game.
                if (input_str == "q")
                {
                    cout << GIVING_UP;
                    return;
                }
                // If player gives a bad input, return to the beginning and ask again.
                if (is_input_proper(input_str) == false)
                {
                    continue;
                }

                // Split those coords. Add into string.
                vector<string> coords = split(input_str);
                string x1 = coords.at(0);
                string y1 = coords.at(1);
                string x2 = coords.at(2);
                string y2 = coords.at(3);

                // Check if flip is allowed. If not, return to the beginning and
                // ask coords again.
                if (is_flip_allowed(x1, y1, x2, y2, game_board, y_bound, x_bound) == false)
                {
                    continue;
                }
                // Flip is allowed.
                // If true, pairs were found.
                if (flip_and_modify_cards(x1, y1, x2, y2, game_board, player, i))
                {
                    // Remove the found pair from the cards that are in the game.
                    cards -= 2;
                    // Player found a pair, so player can continue.
                    continue;
                }
                // Next player is at turn if pair was not found.
                break;
            }
        }
    }
}
int main()
{

    Game_board_type game_board;
    unsigned int factor1 = 1;
    unsigned int factor2 = 1;
    ask_product_and_calculate_factors(factor1, factor2);
    init_with_empties(game_board, factor1, factor2);

    string seed_str = "";
    std::cout << INPUT_SEED;
    std::getline(std::cin, seed_str);
    int seed = stoi_with_check(seed_str);
    init_with_cards(game_board, seed);
    // Init players into a vector.
    vector<Player> players = initialize_players();
    // Print game board before game begins.
    print(game_board);
    // Run game with given values.
    run_game(players, game_board, factor1, factor2);

    return EXIT_SUCCESS;
}

