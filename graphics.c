//graphics.c

#include <stdlib.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include "game.h"
#include "textures.h"
#define HAUT -5
#define BAS 10
#define DROITE 12
#define GAUCHE -3

float ratio = 1.; 

void affichage(void);
void keyboard(char key);
void reshape(int w, int h);
void texturePrint(TEXTURE* tex, int size, int type, float x, float y, float r);
void draw_car(float x, float y, float a);
void draw_radar(float x, float y);
float aim = 20;

int graphics_draw(void) {
	srand(time(NULL));
	int argc; char **argv; 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(900,500);
	ratio = (float) 900 / (float) 500 ;
	glutCreateWindow("Destructeur 2 Radar");


	glutDisplayFunc(affichage);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(affichage);
	
	glutMainLoop();
}

void affichage(void) {
	
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	if (ratio >= 1.){
		glOrtho(GAUCHE, DROITE, BAS/ratio, HAUT/ratio,-1,1); 
	}else{
		glOrtho(GAUCHE*ratio, DROITE*ratio, BAS, HAUT,-1,1);
	}
	
	texturePrint(fond, SQUARE_S, SQUARE_T, 0, 0, 0);
	texturePrint(road, SQUARE_S, SQUARE_T, 0, 0, 0);
	
	draw_car(0, 5, aim);
	draw_radar(6, 4.3);
	draw_radar(11, 4.3);
	glutSwapBuffers();
}

void reshape(int w, int h) {
	//glViewport(0, 0, w, h);
	//ratio = (float) w / (float) h;
	glutReshapeWindow(900, 500);
}

void keyboard(char key) {
	switch(key) {
		case 'w':
			if (aim < 65)
				aim +=2;
			break;
		case 's':
			if (aim > 2)
				aim -= 2;
			break;
		case 'q':
			glutDestroyWindow(1);
			exit (0);
			break;
	}
}

void texturePrint(TEXTURE* tex, int size, int type, float x, float y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0.0); 
	glRotatef(r, 0.0, 0.0, 1.0);
	glBegin(type);
		for(int i = 0; i < size; i++) {
			glColor3f(tex[i].r, tex[i].g, tex[i].b);
			glVertex2f(tex[i].x, tex[i].y);
		}
	glEnd();
	glPopMatrix();
}

void draw_car(float x, float y, float a) {
	texturePrint(cannon, CANNON_S, CANNON_T, x-0.3, y-0.95, -a);
	texturePrint(car, CAR_S, CAR_T, x, y, 0);
	texturePrint(window1, WINDOW_S, WINDOW_T, x, y, 0);
	texturePrint(window2, WINDOW_S, WINDOW_T, x, y, 0);
	texturePrint(circle, CIRCLE_S, CIRCLE_T, x-1.15, y-0.2, 0);
	texturePrint(circle, CIRCLE_S, CIRCLE_T, x+0.8, y-0.2, 90);
	texturePrint(lamp1, LAMP_S, LAMP_T, x, y, 0);
	texturePrint(lamp2, LAMP_S, LAMP_T, x, y, 0);
}

void draw_radar(float x, float y) {
	texturePrint(radar_cam, SQUARE_S, SQUARE_T, x-0.4, y-2.2, 0);
	texturePrint(radar_cam, SQUARE_S, SQUARE_T, x-0.35, y-1.8, 0);
	texturePrint(radar_mount, SQUARE_S, SQUARE_T, x, y, 0);
	texturePrint(radar_box, SQUARE_S, SQUARE_T, x, y-2, 0);
	
}





