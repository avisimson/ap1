/*
 * ReversiGame.h
 *
 *  Created on: Nov 10, 2017
 *      Author: avi simson
 */
#ifndef REVERSI_REVERSIGAME_H
#define REVERSI_REVERSIGAME_H
#include "Board.h"
#include "Player.h"
#include "GameType.h"
class ReversiGame: public GameType {
    //class has a board, space left in board, 2 matrixes of x,y of
    // possible moves for both players and 2 players.
private:
    Board* board;
    Player* player1;
    Player* player2;
    int space;
    int** possiblePointsone; //possible moves for player1.
    int** possiblePointstwo; //possible moves for player2.
public:
    ReversiGame(char p1, char p2);
    ~ReversiGame();
    void playGame();
    bool HumanplayOneTurn(Player* player);
    bool ComputerplayOneTurn(Player* player);
    void scoreGame();
    void checkPossibleMoves(Player* player);
    bool checkUp(Player* player, int i, int j, bool flip);
    bool checkDown(Player* player, int i, int j, bool flip);
    bool checkLeft(Player* player, int i, int j, bool flip);
    bool checkRight(Player* player, int i, int j, bool flip);
    bool checkUpLeft(Player* player, int i, int j, bool flip);
    bool checkUpRight(Player* player, int i, int j, bool flip);
    bool checkDownLeft(Player* player, int i, int j, bool flip);
    bool checkDownRight(Player* player, int i, int j, bool flip);
};
#endif //REVERSI_REVERSIGAME_H
