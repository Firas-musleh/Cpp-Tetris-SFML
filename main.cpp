#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Player.h"
#include <iostream>
#include <string>
using namespace std;
//initialisation des constantes
const int w = 13, h = 24;
int dim = 24; //taille d un bloc
int width = dim*w, height = dim*h;
int score = 0;
int YvalueBeforeChange = 0;
int unefois = 0; //pour jouer le son rire_diabolique une fois
int gambatte = 0;
int combo = 0;
int piecesList[7][3][3] = {
        {{0,0,0},
         {1,1,0},
         {1,1,0}},
        {{0,0,0},
         {0,1,1},
         {1,1,0}},
        {{0,0,0},
         {1,1,0},
         {0,1,1}},
        {{0,0,0},
         {1,1,1},
         {0,1,0}},
        {{0,1,0},
         {0,1,0},
         {0,1,0}},
        {{0,1,0},
         {0,1,0},
         {0,1,1}},
        {{0,1,0},
         {0,1,0},
         {1,1,0}}
};
int tetris_tab[h][w];
Player player(piecesList, w/2, 0, 0);
void initTetrisTab() {
    for(int i=0; i < h; i++) {
        for(int j=0; j < w; j++) {
            tetris_tab[i][j] = 0;
        }
    }
}
int isGameOver() {
    for(int i=0; i < w; i++) {
        if(tetris_tab[0][i] != 0) {
            return 1;
        }
    }
    return 0;
}
void fusioner(Player* p) {
    //on fusionne l arene avec le joueur
    for(int i=0; i < 3; i++) {
        for(int j=0; j < 3; j++) {
            if(p->matrix[i][j] == 1) {
                int type = p->type;
                if(type == 0) {
                    type = -1; //pour les blocs
                }
                tetris_tab[i+p->y][j+p->x] = type; //pour garder la couleur
            }
        }
    }
    /*for(int i=0; i < h; i++) {
        for(int j=0; j < w; j++) {
            cout << tetris_tab[i][j];
        }
            cout <<  endl;
    }*/
}
int collision(Player* p) {
    //collision entre les restes
      for(int y=0; y < 3; y++) {
           for(int x=0; x < 3; x++) {
                 if(p->matrix[y][x] !=0 && tetris_tab[p->y + y+1][p->x + x] !=0) {
                       return 1;
                  }
            }
      }
    int posYPlusBas = 0; //le cube le plus bas
    for(int i=0; i < 3; i++) {
        for(int j=0; j <3; j++) {
            if(p->matrix[i][j] != 0) {
                posYPlusBas = max(posYPlusBas, i);
            }
        }
    }
     if(p->y+posYPlusBas+1 >= h ) {
        return 1;
     }
      return 0;
}
int collisionMurDroit(Player* p) {
      int posXLePlusDroit = 0;
      for(int y=0; y < 3; y++) {
           for(int x=0; x < 3; x++) {
                 if(p->matrix[y][x] !=0) {
                    posXLePlusDroit = max(posXLePlusDroit, x);
                  }
            }
      }
      if(posXLePlusDroit+p->x >= w-1) {
        cout << "Collision :: Mur Droit" << endl;
        return 1;
      }
      return 0;
}
int collisionMurGauche(Player* p) {
      int posXLePlusGauche = w;
      for(int y=0; y < 3; y++) {
           for(int x=0; x < 3; x++) {
                 if(p->matrix[y][x] !=0) {
                    posXLePlusGauche = min(posXLePlusGauche, x);
                  }
            }
      }
      if(posXLePlusGauche+p->x <= 0) {
        cout << "Collision :: Mur Gauche" << endl;
        return 1;
      }
      return 0;
}
int collisionProbableCoinRestes(Player* p) {
    //stop le mouvement lors d une collision
      for(int y=0; y < 3; y++) {
           for(int x=0; x < 3; x++) {
                 if(p->matrix[y][x] !=0) {
                        if(tetris_tab[p->y+y][p->x + x + 1] !=0 || tetris_tab[p->y+y][p->x + x - 1] !=0) {
                                cout << "Collision :: Brique" << endl;
                                 return 1;
                        }
                  }
            }
      }
      return 0;
}
void goDownHighSpeed(Player* p) {
    Player tmp = *p;
    while(collision(&tmp) == 0) {
        tmp.y++;
    }
    tmp.y--;
    p->y = tmp.y;
}
void echangeLigne(int i, int j) {
    for(int x=0; x < w; x++) {
        int tmp = tetris_tab[i][x];
        tetris_tab[i][x] = tetris_tab[j][x];
        tetris_tab[j][x] = tmp;
    }
}
void testGagnant(sf::Sound* audioCombo) {
    int n = 0;
    for(int i=0; i < h; i++) {
        int sum = 0;
        for(int j=0; j < w; j++) {
            if(tetris_tab[i][j] != 0) {
                sum++;
            }
        }
        if(sum == w) {
            n++;
            //la ligne i est gagnante
            for(int u=0; u < w; u++) {
                tetris_tab[i][u] = 0;
            }
            for(int ligne = i; ligne > 0 ; ligne--) {
                echangeLigne(ligne, ligne-1);
            }
        }
    }
    if(n > 0) {
        score = score + 100 * pow(2,n);
        audioCombo->play();
        cout << "Ligne GAGNANTE x " << n << endl;
    }
}
int rotationImpossible(Player* p) {
//A FAIRE
//A FAIRE
//A FAIRE
// ROTAION SUR MUR
// +ROTATION ENTRE DES PIECES
    Player tmp = *p;
    tmp.rotate(); //on va checker ici
    //collisionMurDroit(&tmp) + collisionMurGauche(&tmp)
    int sum = collision(&tmp);
    if(collisionMurGauche(&tmp) > 0) {
        return 1;
    }
    if(sum > 0) return 1;
    return 0;
}
void setRandomPiece() {
    int t = std::rand() % 7;
    player.init(piecesList, w/2, -2, t);
}

void newGame() {
    score = 0;
    unefois = 0;
    setRandomPiece();
    initTetrisTab();
}
char* intEnChaine(int num) {
    // A REMARQUER!!!!
    //on fait un malloc et on est SAUF
    //sinon on devrait utiliser un const char*
    //SINON , crash!!
    char* a = (char*) malloc(sizeof(char) * 50);
    sprintf(a, "%i", num);
    return a;
}
int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "TETRIS by afmika (Nyah Nyah Tetris)",
                            sf::Style::Titlebar | sf::Style::Close /*on affiche que le titre et la fermeture X*/
                            );
    newGame();
    /** TEXTES **/
    sf::Font font;
    if(!font.loadFromFile("nyah_tetris_afmika_data/AUBREY1__.af")) {
        cout << "Erreur font" << endl;
    }
    sf::Text game_over_text, new_game_text, score_text;
    game_over_text.setFont(font);
    new_game_text.setFont(font);
    score_text.setFont(font);
    game_over_text.setCharacterSize(35);
    game_over_text.setColor(sf::Color::Red);
    new_game_text.setCharacterSize(20);
    new_game_text.setColor(sf::Color::Green);
    score_text.setCharacterSize(20);
    score_text.setColor(sf::Color::Cyan);
    game_over_text.setString("Game Over");
    new_game_text.setString("New Game");
    score_text.setString(intEnChaine(0));
    /** IMAGES **/
    sf::Texture texture, monika_texture, background_texture, grille_texture;
    texture.loadFromFile("nyah_tetris_afmika_data\\tetrominos.af");
    monika_texture.loadFromFile("nyah_tetris_afmika_data\\monika.af");
    background_texture.loadFromFile("nyah_tetris_afmika_data\\bg.af");
    grille_texture.loadFromFile("nyah_tetris_afmika_data\\grille.af");

    sf::Sprite image(texture), monika(monika_texture), bg(background_texture), grille(grille_texture);

    /** SONS **/
    sf::Music rire; //lecture a la volee
    if(!rire.openFromFile("nyah_tetris_afmika_data\\rire_diabolique.af")) {
        cout << "erreur rire_diabolique.wav" << endl;
    }

    /** BUFFER avantage : chargés dans la RAM**/
    sf::SoundBuffer fusion_song_buffer, touche_song_buffer, combo_song_buffer, onii_chan_buffer, gambatte_buffer;
    if(!fusion_song_buffer.loadFromFile("nyah_tetris_afmika_data\\fusion.af")) {
        cout << "erreur fusion.wav" << endl;
    }
    if(!touche_song_buffer.loadFromFile("nyah_tetris_afmika_data\\touche.af")) {
        cout << "erreur touche.wav" << endl;
    }
    if(!combo_song_buffer.loadFromFile("nyah_tetris_afmika_data\\combo.af")) {
        cout << "erreur combo.wav" << endl;
    }
    if(!onii_chan_buffer.loadFromFile("nyah_tetris_afmika_data\\onii_chan_sucks.af")) {
        cout << "erreur onii_chan_sucks.wav" << endl;
    }
    if(!gambatte_buffer.loadFromFile("nyah_tetris_afmika_data\\gambattemasu.af")) {
        cout << "Erreur gambattemasu.wav" << endl;
    }
    sf::Sound fusion_effect, touche_effect, combo_effect, onii_chan_effect, gambatte_effect;
    fusion_effect.setBuffer(fusion_song_buffer);
    touche_effect.setBuffer(touche_song_buffer);
    combo_effect.setBuffer(combo_song_buffer);
    onii_chan_effect.setBuffer(onii_chan_buffer);
    gambatte_effect.setBuffer(gambatte_buffer);
    while(window.isOpen()) {
        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
            if(e.type == sf::Event::KeyPressed && player.y > 0) {
                int code = (int) e.key.code;
                touche_effect.play();
                switch(code) {
                    case 73: //haut
                        if(rotationImpossible(&player) == 0) player.rotate();
                        break;
                    case 74://bas
                        goDownHighSpeed(&player);
                        break;
                    case 71://gauche
                        if(collisionMurGauche(&player) + collisionProbableCoinRestes(&player) == 0) {
                                player.y = YvalueBeforeChange;
                                player.x--;
                        }
                        break;
                    case 72://droite
                        if( collisionMurDroit(&player) + collisionProbableCoinRestes(&player) == 0) {
                                player.y = YvalueBeforeChange;
                                player.x++;
                        }
                        break;
                }
            }
            if(e.type == sf::Event::MouseButtonPressed && isGameOver() == 1) {
                newGame();
            }
        }
        window.draw(score_text);
        window.display();
        window.clear(sf::Color::Black);
        /**DESSINER LA TAB DES PIECES FIGEES**/
        testGagnant(&combo_effect);
        for(int i=0; i < h; i++) {
            for(int j=0; j < w; j++) {
                int k = tetris_tab[i][j];
                if(k != 0) {
                    k = (k < 0) ? 0 : k; //cas brique 4x4
                   /* image.setTextureRect(sf::IntRect(k*dim, 0, dim, dim));
                    image.setPosition(j*dim, i*dim);
                    window.draw(image);*/
                   bg.setTextureRect(sf::IntRect(j*dim, i*dim, dim, dim));
                    bg.setPosition(j*dim, i*dim);
                    window.draw(bg);
                }
            }
        }
        window.draw(grille);
        if(isGameOver() == 0) {
            /** DESSINER UNE PIECE **/
            int type = player.type;
            for(int i=0; i < 3; i++) {
                for(int j=0; j < 3; j++) {
                    if(player.matrix[i][j] != 0) {
                        image.setTextureRect(sf::IntRect(type*dim,0,dim,dim));
                        image.setPosition(j*dim + dim * player.x, i*dim + dim * player.y);
                        window.draw(image);
                    }
                }
            }
            /**SE CHARGE DE LA DESCENTE **/
            YvalueBeforeChange = player.y;
            player.y++;
            if(collision(&player) == 1) {
                fusioner(&player);
                score += 50;
                fusion_effect.play();
                setRandomPiece();
                gambatte = 0;
            }
            //cout << "score :: " << score << " " << endl;
            if(score % 1000 == 0 && score != 0 && gambatte == 0) {
                gambatte_effect.play();
                gambatte = 1;
            }
        } else {
            game_over_text.setPosition(width/2-80, height/2);
            new_game_text.setPosition(width/2-40, height/2 + 50);
            if(unefois == 0) {
                    cout << "Game Over" << endl;
                    onii_chan_effect.play();
                    rire.play();
            }
            unefois = 1;
            window.draw(monika);
            window.draw(game_over_text);
            window.draw(new_game_text);
        }
        score_text.setString(intEnChaine(score));
        window.draw(score_text);
        sf::sleep(sf::milliseconds(1000/4));
    }
    return 0;
}
