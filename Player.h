/*
 * Player.h
 *
 *  Created on: Nov 10, 2017
 *      Author: avi simson
 */
#ifndef REVERSI_PLAYER_H
#define REVERSI_PLAYER_H
class Player {
    //class defines a player in a game. the player has name and type.
    private:
        char name;
        char type; // 'C' for computer, 'H' for user.
        int num; //number of player 1 or 2.
    public:
        Player(char name1, char type1, int num1);
        ~Player();
        char getName();
        char getType();
        void setType(char c);
        int getNum();
};
#endif //REVERSI_PLAYER_H
