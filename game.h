//game.h

#ifndef GAME_H
#define GAME_H


void affichage(void);

void keyboard(char key);
void reshape(int w, int h);
void texturePrint(TEXTURE* tex, int size, int type, float x, float y, float r);
void draw_car(float x, float y);
void draw_radar(float x, float y, int v, int e, int ex, int h);
void draw_bullet(float x, float y, int v);
void draw_text(char *string,float x,float y);






#endif
