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
    ImVec2 boardOrigin(125.0f, 125.0f);
    const float spacer = 25.0f;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            ImVec2 pos(
                boardOrigin.x + (col * squareSize) + (col * spacer),
                boardOrigin.y + (row * squareSize) + (row * spacer)
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
    if (game_over) {
        return false;
    }
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
    //Create a Bit for the new piece
    //Line it up with the holder
    //Assign it to the holder
    //Pass because we placed a piece
    int currentPlayerIndex = getCurrentPlayer()->playerNumber();
    Bit* new_piece = PieceForPlayer(currentPlayerIndex);
    new_piece->setPosition(holder->getPosition().x, holder->getPosition().y);
    holder->setBit(new_piece);

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
    game_over = false;
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index) const
{
    // convert index to x,y:
    int y = index / 3;
    int x = index % 3;

    // if no bit at that location return nullptr
    if (!_grid[y][x].bit()) {
        return nullptr;
    }
    // otherwise return the owner of the bit at that location
    Player* owner = _grid[y][x].bit()->getOwner();
    return owner;
}

Player* TicTacToe::checkForWinner() 
{
    int winners[8][3] = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};

    for (int i = 0; i < 8; i++) {
        Player* p = ownerAt(winners[i][0]);
        if (p &&
            p == ownerAt(winners[i][1]) &&
            p == ownerAt(winners[i][2])) {
            return p;
        }
    }
    return nullptr;
}

bool TicTacToe::isBoardFull() const {
    for (int i = 0; i < 9; i++) {
        if (!ownerAt(i)) {
            return false;
        }
    }
    return true;
}

bool TicTacToe::checkForDraw()
{
    return checkForWinner() == nullptr && isBoardFull();
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

        _grid[y][x].destroyBit();
        int num_state = s[i] - '0';
        
        if (num_state == 0) {
            _grid[y][x].setBit(nullptr);
            continue;
        }

        Bit* new_piece = PieceForPlayer(num_state-1);
        new_piece->setPosition(_grid[y][x].getPosition().x, _grid[y][x].getPosition().y);
        _grid[y][x].setBit(new_piece);
    }
    //game_over = false;
}



int TicTacToe::evaluate(Player* aiPlayer)
{
    Player* winner = checkForWinner();

    if (winner == aiPlayer)
        return 1;    // AI win
    if (winner != nullptr)
        return -1;   // AI loss

    return 0;        // draw or ongoing
}


void TicTacToe::makeMove(int index, Player* player) {
    if (index < 0 || index >= 9)
        return;

    if (ownerAt(index))
        return;

    Bit* new_piece = PieceForPlayer(player->playerNumber());
    if (!new_piece){return;}

    int y = index / 3;
    int x = index % 3;

    new_piece->setPosition(_grid[y][x].getPosition().x, _grid[y][x].getPosition().y);
    _grid[y][x].setBit(new_piece);
}

Player* TicTacToe::getOtherPlayer(Player* p)
{
    if (!p || _players.size() < 2)
        return nullptr;

    int idx = p->playerNumber();   // 0 or 1
    int otherIdx = 1 - idx;        // flips 0↔1

    return _players[otherIdx];
}

void TicTacToe::extractBoard(int board[9]) const
{
    for (int i = 0; i < 9; i++) {
        Player* owner = ownerAt(i);
        if (!owner)
            board[i] = 0;
        else
            board[i] = owner->playerNumber() + 1;
    }
}

int checkWinner(const int board[9])
{
    static int wins[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for (auto& w : wins) {
        int a = w[0], b = w[1], c = w[2];
        if (board[a] != 0 &&
            board[a] == board[b] &&
            board[a] == board[c]) {
            return board[a];
        }
    }
    return 0;
}

bool boardFull(const int board[9])
{
    for (int i = 0; i < 9; i++)
        if (board[i] == 0)
            return false;
    return true;
}

int TicTacToe::negamax(int board[9], int currentPlayer, int aiPlayer)
{
    int winner = checkWinner(board);

    if (winner != 0)
        return (winner == aiPlayer) ? 10 : -10;

    bool full = true;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            full = false;
            break;
        }
    }

    if (full)
        return 0;

    int bestScore = -1000;

    for (int i = 0; i < 9; i++) {
        if (board[i] != 0)
            continue;

        board[i] = currentPlayer;
        int score = -negamax(board, 3 - currentPlayer, aiPlayer);
        board[i] = 0;

        bestScore = std::max(bestScore, score);
    }

    return bestScore;
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI()
{
    

    int board[9];
    extractBoard(board);

    int aiPlayer = AI_PLAYER + 1;
    int opponent = 3 - aiPlayer;

    int bestScore = -1000;
    int bestMove = -1;

    for (int i = 0; i < 9; i++) {
        
        if (board[i] != 0)
            continue;

        board[i] = aiPlayer;
        int score = -negamax(board, opponent, aiPlayer);
        board[i] = 0;

        if (score > bestScore) {
            bestScore = score;
            bestMove = i;
        }
    }
    std::cout << bestMove << std::endl;
    if (bestMove != -1) {
        makeMove(bestMove, getCurrentPlayer());
    }
    
    endTurn();
}




