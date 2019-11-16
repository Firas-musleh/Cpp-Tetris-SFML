#ifndef PLAYER_H
#define PLAYER_H


class Player
{
    public:
        int x, y;
        int type; //couleur et type
        int matrix[3][3]; //piece
        Player(int piecesList[7][3][3], int x, int y, int type);
        void init(int piecesList[7][3][3], int x, int y, int type); //initialiser
        void rotate(); //retourner
};

#endif // PLAYER_H
