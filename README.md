Version 1.0.0
Built on Windows

This is Tictactoe!
Forked from gdevine

#1: Added get methods for Square col and row for testing purposes but that is all that I changed outside of TicTacToe.cpp

#2: I noticed that pieceForPlayer wasnt choosing the right player so I changed what it was looking for from 1 to 0 as well as the winner dialogue that pops up in Application.cpp

Initial comments of instructions were replaced with comments describing the functions for readibility


Patch Notes!! v 1.0.1
Fixed a bug where a player could place 2 pieces on the same turn
    - Altered the spacing between the boxes
Fixed a bug that let the game continue after a player had won
    - Added new variable (game_over) in the class TicTacToe that disallows this