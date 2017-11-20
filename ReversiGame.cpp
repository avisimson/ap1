/*
 * ReversGame.h
 *
 *  Created on: Nov 10, 2017
 *      Author: avi simson
 */
#include <iostream>
#include <sstream>
#include "ReversiGame.h"
using namespace std;
//constructor, initialize board, possible points matrix and players
//parameters-p1, p2 are player 1 and 2 names.
ReversiGame :: ReversiGame(char p1, char p2) {
    player1 = new Player(p1, 'H', 1); //HUMAN PLAYER
    player2 = new Player(p2, 'C', 2); //COMPUTER PLAYER
    board = new Board(p1, p2);
    space = ((board->getSize()) * board->getSize()) - 4;
    //allocate memory for possible points matrix for computer and human players,
    // and initialize their values to -1.
    possiblePointsone = new int*[space];
    for (int i = 0; i < space; i++) {
        possiblePointsone[i] = new int[2];
        for(int j = 0; j < 2; j++) {
            possiblePointsone[i][j] = -1;
        }
    }
    possiblePointstwo = new int*[space];
    for (int i = 0; i < space; i++) {
        possiblePointstwo[i] = new int[2];
        for(int j = 0; j < 2; j++) {
            possiblePointstwo[i][j] = -1;
        }
    }
}
//destructor-releases all memory from system.
ReversiGame :: ~ReversiGame() {
    delete player1;
    delete player2;
    for (int i = 0; i < ((board->getSize()) * board->getSize()) - 4; i++) {
        delete possiblePointsone[i];
    }
    delete possiblePointsone;
    for (int i = 0; i < ((board->getSize()) * board->getSize()) - 4; i++) {
        delete possiblePointstwo[i];
    }
    delete possiblePointstwo;
    delete board;
}
//function runs reversi game until both players can't play or board is full.
//at the end of the game function prints the score and the winner.
void ReversiGame :: playGame() {
    int currentTurn = 1; //tells who's turn is it player1/player2.
    int cantPlay = 0; //counts if both players cant play to finish game.
    bool b = true;
    board->printBoard();
    //check if player1 and player2 initialized as C or H type to play.
    if(player1->getType() != 'C' && player1->getType() != 'H')
    {
        cout << "Game cannot be initialized, player1 type is wrong" << endl;
        return;
    }
    if(player2->getType() != 'C' && player2->getType() != 'H')
    {
        cout << "Game cannot be initialized, player2 type is wrong" << endl;
        return;
    }
    //Game loop.
    while(space > 0 && cantPlay < 2) {
        if(currentTurn == 1) {
            if(player1->getType() == 'C') {
                b = ComputerplayOneTurn(player1);
            } else {
                b = HumanplayOneTurn(player1);
            }
            currentTurn++;
            if (b == false) {
                cantPlay++;
            } else {
                cantPlay = 0;
                space--;
            }
        } else if (cantPlay < 2) {
            if(player2->getType() == 'C') {
                b = ComputerplayOneTurn(player2);
            } else {
                b = HumanplayOneTurn(player2);
            }
            currentTurn--;
            if (b == false) {
                cantPlay++;
            } else {
                cantPlay = 0;
                space--;
            }
        }
    }
    //game ended, print score of the game.
    scoreGame();
}
//parameters: player is the char sign for the player which is his turn.
//function plays on turn of reversi by checking possible moves for the player,
//and plays the turn for him.
//return: true is player played turn, false if not.
bool ReversiGame :: HumanplayOneTurn(Player* player) {
    checkPossibleMoves(player);
    int** possiblePoints;
    //create new matrix that will point on the correct matrix according to
    // if player is player 1 or 2.
    if(player->getNum() == 1) {
        possiblePoints = possiblePointsone;
    } else if(player->getNum() == 2) {
        possiblePoints = possiblePointstwo;
    } else {
        cout << player->getName() <<
             " is not initialized as player1 or player2" << endl;
        return false;
    }
    if (possiblePoints[0][0] == -1) { //player has no possible moves, cant play
        cout << player->getName()
             << " You have no possible moves, turn passed." << endl;
        return false;
    } else { //player can play
        cout << player->getName() << ": it's your move" << endl;
        cout << "Your possible moves: ";
        int index = 0;
        //print possible moves for player.
        while(possiblePoints[index][0] > -1) {
            cout << "(" << possiblePoints[index][0] <<
                 "," << possiblePoints[index][1] << ")";
            if(possiblePoints[index + 1][0] != -1) {
                cout << ",";
            }
            index++;
        }
        cout << "" << endl;
        cout << "" << endl;
        int row, col, i, counter = 0;
        string row1, col1;
        //player enters row,col and the game makes sure that he played,
        //a legal move.
        while (counter == 0) {
            cout << "Please enter your move- enter row,col: ";
            cin >> row1 >> col1;
            istringstream buffer(row1);
            buffer >> row;
            istringstream buffer2(col1);
            buffer2 >> col;
            for(i = 0; i < space; i++) {
                if(row == possiblePoints[i][0]) {
                    if(col == possiblePoints[i][1]) {
                        break;
                    }
                }
            }
            if(i >= space) {
                cout << "Wrong input!" << endl;
            } else {
                counter = 1;
                board->setBoard(row,col,player->getName());
                checkUp(player, row, col, true);
                checkDown(player, row, col, true);
                checkLeft(player, row, col, true);
                checkRight(player, row, col, true);
                checkUpLeft(player, row, col, true);
                checkUpRight(player, row, col, true);
                checkDownLeft(player, row, col, true);
                checkDownRight(player, row, col, true);
                i = 0;
                //reset possible moves for next turn.
                while(possiblePoints[i][0] != -1) {
                    possiblePoints[i][0] = -1;
                    possiblePoints[i][1] = -1;
                    i++;
                }
                board->printBoard();
                cout << player->getName() << " played: ("
                     << row << "," << col << ")" << endl;
                return true;
            }
        }
    }
}
//parameters: player is the char sign for the player which is his turn.
//function plays on turn of reversi by checking possible moves for the player,
//and plays the turn for him.
//return: true is player played turn, false if not.
bool ReversiGame :: ComputerplayOneTurn(Player* player) {
    checkPossibleMoves(player);
    int** possiblePoints;
    //create new matrix that will point on the correct matrix according to
    // if player is player 1 or 2.
    if(player->getNum() == 1) {
        possiblePoints = possiblePointsone;
    } else if(player->getNum() == 2) {
        possiblePoints = possiblePointstwo;
    if (possiblePoints[0][0] == -1) { //player has no possible moves, cant play
        cout << player->getName()
             << " You have no possible moves, turn passed." << endl;
        return false;
    } else { //player can play
        int index = 0, min = board->getSize() * board->getSize(),
                globalMinIndex = 0;
        while (possiblePoints[index][0] != -1) {
            if()
        }
    }
}
//function checks who is the winner and prints the winner and the score.
//run on the board matrix and count how much player1 shows up and how much
//player 2 shows up.
void ReversiGame :: scoreGame() {
    int count1 = 0, count2 = 0, i, j;
    for(i = 1; i <= board->getSize(); i++) {
        for(j = 1; j <= board->getSize(); j++) {
            if(board->getBoard()[i][j] == player1->getName()) {
                count1++;
            } else if(board->getBoard()[i][j] == player2->getName()) {
                count2++;
            }
        }
    }
    cout << player1->getName() << ": Your score is " << count1 << endl;
    cout << player2->getName() << ": Your score is " << count2 << endl;
    if(count1 > count2) {
        cout << "The Winner is- " << player1->getName();
    } else if(count2 > count1) {
        cout << "The Winner is- " << player2->getName();
    } else {
        cout << "The Game ended in a DRAW." << endl;
    }
}
//function gets a player as a parameter and update the possible moves of
//the player on the human/computer possible points array.
void ReversiGame :: checkPossibleMoves(Player* player) {
    int i, j, count = 0;
    for(i = 1; i <= board->getSize(); i++) {
        for(j = 1; j <= board->getSize(); j++) {
            if(board->getBoard()[i][j] == ' ') {
                if(checkUp(player, i, j, false)
                   || checkDown(player, i, j, false)
                   || checkLeft(player, i, j, false)
                   || checkRight(player, i, j, false)
                   || checkUpLeft(player, i, j, false)
                   || checkUpRight(player, i, j, false)
                   || checkDownLeft(player, i, j, false)
                   || checkDownRight(player, i, j, false)) {
                    if(player->getType() == 'C') {
                        possibleCPoints[count][0] = i;
                        possibleCPoints[count][1] = j;
                    } else {
                        possibleHPoints[count][0] = i;
                        possibleHPoints[count][1] = j;
                    }
                    count++;
                }
            }
        }
    }
}
//function check for  possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkUp(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    if(i == 1 || i == 2) {
        return false;
    } else {
        i--;
        while(i > 0) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i + count2, j, player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            i--;
        }
        return false;
    }
}
//function check for  possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkDown(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    if(i == board->getSize() - 1 || i == board->getSize()) {
        return false;
    } else {
        i++;
        while(i <= board->getSize()) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i - count2, j, player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            i++;
        }
        return false;
    }
}
//function check for possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkLeft(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    if(j == 1 || j == 2) {
        return false;
    } else {
        j--;
        while(j > 0) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i, j + count2, player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            j--;
        }
        return false;
    }
}
//function check for possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkRight(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    if(j == board->getSize() - 1 || j == board->getSize()) {
        return false;
    } else {
        j++;
        while(j <= board->getSize()) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i, j - count2, player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            j++;
        }
        return false;
    }
}
//function check for possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkUpLeft(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    if(j == 1 || j == 2 || i == 1 || i == 2) {
        return false;
    } else {
        j--;
        i--;
        while(j > 0 && i > 0) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i + count2, j + count2,
                                            player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            j--;
            i--;
        }
        return false;
    }
}
//function check for possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkUpRight(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    int size = board->getSize();
    if(j == size - 1 || j == size || i == 1 || i == 2) {
        return false;
    } else {
        j++;
        i--;
        while(j <= size && i > 0) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i + count2, j - count2,
                                            player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            j++;
            i--;
        }
        return false;
    }
}
//function check for possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkDownLeft(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    int size = board->getSize();
    if(j == 1 || j == 2 || i == size - 1 || i == size) {
        return false;
    } else {
        j--;
        i++;
        while(j > 0 && i <= size) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i - count2, j + count2,
                                            player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            j--;
            i++;
        }
        return false;
    }
}
//function check for possible moves for an i,j point,
//and flips disc if boolean flip is true.
//return: true if possible move or false if not.
//par:player is current player's turn, (i,j) are row and col of
// the point being checked. flip says weather to flip disc or not.
bool ReversiGame :: checkDownRight(Player* player, int i, int j, bool flip) {
    int count2 = 0; //counts other player showings.
    int size = board->getSize();
    if(j == size - 1 || j == size || i == size - 1 || i == size) {
        return false;
    } else {
        j++;
        i++;
        while(j <= size && i <= size) {
            if(board->getBoard()[i][j] != player->getName()
               && board->getBoard()[i][j] != ' ') {
                count2++;
            }
            if(board->getBoard()[i][j] == player->getName()) {
                if(count2 > 0) {
                    //change disc if flip is true.
                    if(flip) {
                        while(count2 > 0) {
                            board->setBoard(i - count2, j - count2,
                                            player->getName());
                            count2--;
                        }
                    }
                    return true;
                }
                return false;
            }
            if(board->getBoard()[i][j] == ' ') {
                return false;
            }
            j++;
            i++;
        }
        return false;
    }
}