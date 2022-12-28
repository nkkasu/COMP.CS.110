## Pairs

A pairs game
Program executes a pairs (memory) game. Game can have a varying amount of cards
and players by changing hard coded constat CARDCOUNT in mainwindow.cpp.

In the beginning of the game player is prompted of player names who
are going to be playing. At least one player has to be added to start the game by
pressing the Start game-button. When game begins, timer starts. Player has to click
2 different cards on the screen to check whether they are pairs or not. If they are not
pairs, turn switches to another player OR stays on the player if only 1 player is playing.
If cards are pairs, player can continue until player doesn't find a pair. Found pairs are added
to current points of player. Game will display how many pairs are found and who is in turn
currently to make game experience smoother. Game continues, until all pairs are found and
screen is empty of pairs. Then game will declare who the winner is. There can also be a tie,
which will be declared differently. When game ends, time also stops. Card's hidden side letters are
letters from alphabet, starting from A and ending to Z.

NOTE:
Upper limit for cards is 30 cards! Also, 13 and 26 cards are not recommended, since position of cards
are calculated on grid based on factors. Card class is pretty empty, but idea was to add images to all cards
which would have made Card-class more useful.

![image](https://user-images.githubusercontent.com/100607632/209875511-391c095e-91e2-4ab3-b2c7-ca744b8482a2.png)
![image](https://user-images.githubusercontent.com/100607632/209876132-a618cdf2-5ae3-464f-9d02-be464d7ce14e.png)
