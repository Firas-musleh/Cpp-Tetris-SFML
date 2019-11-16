#include "Player.h"
#include <iostream>
using namespace std;
Player::Player(int piecesList[7][3][3], int x, int y, int type) {
    this->init(piecesList, x, y, type);
}
void Player::init(int piecesList[7][3][3], int x, int y, int type) {
    this->x = x;
    this->y = y;
    this->type = type;
    for(int i=0; i < 3; i++) {
          for(int j=0; j < 3; j++) {
              this->matrix[i][j] = piecesList[type][i][j];
         }
    }
}
void Player::rotate() {
    if(this->type != 0) {
            //si non carre uniquement
        for(int j=0; j < 3; j++) {
            for(int i=0; i < j; i++) {
                int tmp =  this->matrix[i][j];
                this->matrix[i][j] = this->matrix[j][i];
                this->matrix[j][i] = tmp;
            }
        }
        for(int i=0; i < 3; i++) {
                int tmp = this->matrix[0][i];
                this->matrix[0][i] = this->matrix[2][i];
                this->matrix[2][i] = tmp;
        }
    }
}
