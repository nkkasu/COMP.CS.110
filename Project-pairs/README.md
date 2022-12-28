## Pairs

A pairs game

Program executes a pairs (memory) game. Game has a varying amount of cards
and players. In the beginning of the game player is asks for a seed value,
since all cards are drawn randomly on the game board.
Game then asks how many players are going to play the game, and asks
to list the players. Every round player in turn gives coordinates of
two cards (x1, y1), (x2, y2). After coordinates are given, game checks
whether coordinates are valid, and then turns the cards around. Cards must
be found in the game board, as well as player can't choose two of the same
coordinate. Game checks whether cards are a pair or not. If cards were a pair,
player is given more score and cards are removed from the game board player
can also continue his turn and try finding more pairs. If cards were not a pair,
cards are turned back hidden. Player doesn't get any points and turn is given
to the next player. Cards are depicted as letters beginning from A to as far
as there are cards. When printing the game board, visible card is shown as
#, whereas hidden one as dot (.). Game ends when all pairs have been found,
and the game board is empty. The program tells who has/have won, as well as most
pairs collected.