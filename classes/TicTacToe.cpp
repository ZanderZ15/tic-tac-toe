#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// -----------------------------------------------------------------------------

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 0 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // call setNumberOfPlayers to 2
    setNumberOfPlayers(2);

    //set up the game options so the mouse knows to draw a 3x3 grid
    // _gameOptions has a rowX and rowY property we set to 3
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;

    //setup 3x3 array in _grid with the correct position of the square, and load the "square.png" sprite for each square
    const float squareSize = 80.0f;
    ImVec2 boardOrigin(150.0f, 150.0f);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            ImVec2 pos(
                boardOrigin.x + col * squareSize,
                boardOrigin.y + row * squareSize
            );
            _grid[row][col].initHolder(pos, "square.png", col, row);
        }
    }
    
    // finally call startGame to, well, start
    startGame();

}

//
//
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // 1) If the holder doesn't exist fail
    if (!holder) {
        return false;
    }

    // 2) Check that the holder is empty, if not fail
    if (holder->bit()) {
        return false;
    }

    // 3) Place the current player's piece on this holder

    //This figures out whose turn it is 
    int currentPlayerIndex = getCurrentPlayer()->playerNumber();

    //Create a Bit for the new piece
    Bit* new_piece = PieceForPlayer(currentPlayerIndex);

    //Line it up with the holder
    new_piece->setPosition(holder->getPosition().x, holder->getPosition().y);

    //Assign it to the holder
    holder->setBit(new_piece);

    //Pass because we placed a piece
    return true; 
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    // loop through the 3x3 array and destroy the Bits on each square
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            Square *holder = &_grid[row][col];
            if(holder->bit()) {
                holder->destroyBit();
            }
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // convert index to x,y:
    int y = index / 3;
    int x = index % 3;

    // if no bit at that location return nullptr
    if (!_grid[x][y].bit()) {
        return nullptr;
    }
    // otherwise return the owner of the bit at that location
    Player* owner = _grid[x][y].bit()->getOwner();
    return owner;
}

Player* TicTacToe::checkForWinner()
{
    // Check all winning triples
    // if any of them have the same owner return that player
    // winning triples are:
    int winners[8][3] = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};

    for (int i = 0; i < 8; i++) {
        int a = winners[i][0];
        int b = winners[i][1];
        int c = winners[i][2];
        if (ownerAt(a) == ownerAt(b) && ownerAt(b) == ownerAt(c)) { //the same as checking a == b == c == a
            return ownerAt(a);
        }
    }

    // if you find a winning triple, return the player who owns that triple
    // otherwise return nullptr

    return nullptr;
}

bool TicTacToe::checkForDraw()
{   
    // check for a winner
    if (checkForWinner()) {
        return false;
    }
    // if no winner check if there is an empty square
    // if any square is empty, return false
    // otherwise return true

    for (int i = 0; i < 9; i++) {
        if (!ownerAt(i)) {
            return false;
        }
    }
    
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    // Characters are '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order is left-to-right, top-to-bottom
    
    //Initialize a starting string
    //add characters to represent empty, player 1 or player 2
    std::string state = "";
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (!_grid[row][col].bit()) {
                state += '0';
                continue;
            } else {
                int player = _grid[row][col].bit()->getOwner()->playerNumber() + 1;
                state += std::to_string(player);
            }
        }

    }
    // finally, return the constructed string
    return state;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // given a string, set the board state to accurately represent it
    // string is 9 characters long, one for each square
    // each character is either '0' for empty, '1' for player 1 (X), or '2' for player 2 (O)
    // the order is left-to-right, top-to-bottom


    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, a piece is created for that player and set in the square
    // the piece is positioned at the holder's position

    for (int i = 0; i < 9; i++) {
        int y = i / 3;
        int x = i % 3;

        int num_state = s[i] - 0;
        if (num_state == 0) {
            _grid[x][y].setBit(nullptr);
            continue;
        }

        Bit* new_piece = PieceForPlayer(num_state);
        new_piece->setPosition(_grid[x][y].getPosition().x, _grid[x][y].getPosition().y);
        _grid[x][y].setBit(new_piece);
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    // we will implement the AI in the next assignment!
}

