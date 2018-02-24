/* DESTRUCTEUR 2 RADAR
 * Author: Iacopo Sprenger
 * Version: 1.0
 * Date: 24.02.2018
 * Filename: game.c
 * */

#include <stdlib.h>
#include <GLUT/glut.h>
// use #include <GL/glut.h> for linux
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "calculations.h"
#include "constants.h"
#include "textures.h"
#include "game.h"

float ratio; 

int main(int argc, char **argv) {
	srand(time(NULL));
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(900,500);
	ratio = (float) 900 / (float) 500 ;
	glutCreateWindow("Destructeur 2 Radar"); 
	glutDisplayFunc(affichage);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(FLEN, calculate, 0);
	glutMainLoop();
}

void affichage(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_POLYGON_SMOOTH);
	
	if (ratio >= 1.){
		glOrtho(GAUCHE, DROITE, BAS/ratio, HAUT/ratio,-1,1); 
	}else{
		glOrtho(GAUCHE*ratio, DROITE*ratio, BAS, HAUT,-1,1);
	}
	texturePrint(fond, SQUARE_S, SQUARE_T, 0, 0, 0);
	texturePrint(road, SQUARE_S, SQUARE_T, 0, 0, 0);
	for(int i = 0; i < MAX_RADARS; i++)
		draw_radar(radars[i].x, radars[i].y, radars[i].visible, radars[i].intact, radars[i].e_step, radars[i].low);
	for(int i = 0; i < MAX_BULLETS; i++)
		draw_bullet(bullets[i].x, bullets[i].y, bullets[i].visible);
	draw_car(0, 5);
	if(flash == 1) {
		texturePrint(star, STAR_S, STAR_T, 0.6, 2.15, 0);
		flash = 0;
	}
	if(flash == 2) {
		texturePrint(star, STAR_S, STAR_T, 0.6, 3.15, 0);
		flash = 0;
	}
	char* str[25];
	sprintf(str, "Score: %d", score);
	draw_text(str, -2.5, -2.);
	
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
				aim +=AIM_SPEED;
			break;
		case 's':
			if (aim > 5)
				aim -= AIM_SPEED;
			break;
		case ' ':
			if (fire == 0)
				fire = 1; 
			break;
		case 'p':
			if (ap == 0) {
				ap = 1; 
			}else {
				ap = 0;
			}
			break;
		case 'q':
			glutDestroyWindow(1);
			exit(0);
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

void draw_car(float x, float y) {
	texturePrint(cannon, CANNON_S, CANNON_T, x-0.3, y-0.95, -aim);
	texturePrint(car, CAR_S, CAR_T, x, y, 0);
	texturePrint(window1, WINDOW_S, WINDOW_T, x, y, 0);
	texturePrint(window2, WINDOW_S, WINDOW_T, x, y, 0);
	texturePrint(circle, CIRCLE_S, CIRCLE_T, x-1.15, y-0.2, tire_angle);
	texturePrint(circle, CIRCLE_S, CIRCLE_T, x+0.8, y-0.2, 90+tire_angle);
	texturePrint(lamp1, LAMP_S, LAMP_T, x, y, 0);
	texturePrint(lamp2, LAMP_S, LAMP_T, x, y, 0);
}

void draw_radar(float x, float y, int v, int e, int ex, int h) {
	if(v) {
		if(h == 0) {
			if (e) {
				texturePrint(radar_cam, SQUARE_S, SQUARE_T, x-0.2, y-2.1, 0);
				texturePrint(radar_cam, SQUARE_S, SQUARE_T, x-0.17, y-1.9, 0);
				texturePrint(radar_mount, SQUARE_S, SQUARE_T, x, y, 0);
				texturePrint(radar_box, SQUARE_S, SQUARE_T, x, y-2, 0);
			} else {
				texturePrint(radar_b_mount, SQUARE_S, SQUARE_T, x, y, 0);
			}
			if(ex) {
				int r = rand()%90;
				if(ex<=10)
					texturePrint(explo1, EXPLO_S, EXPLO_T, x, y-2, r);
					texturePrint(explo1, EXPLO_S, EXPLO_T, x, y-2, r+45);
				if(ex>=10)
					texturePrint(explo2, EXPLO_S, EXPLO_T, x, y-2, r);
					texturePrint(explo2, EXPLO_S, EXPLO_T, x, y-2, r+45);
			}
		}
		if(h == 1) {
			if (e) {
				texturePrint(radar_cam, SQUARE_S, SQUARE_T, x-0.2, y-1.1, 0);
				texturePrint(radar_cam, SQUARE_S, SQUARE_T, x-0.17, y-0.9, 0);
				texturePrint(low_radar_mount, SQUARE_S, SQUARE_T, x, y, 0);
				texturePrint(radar_box, SQUARE_S, SQUARE_T, x, y-1, 0);
			} else {
				texturePrint(low_radar_b_mount, SQUARE_S, SQUARE_T, x, y, 0);
			}
			if(ex) {
				int r = rand()%90;
				if(ex<=10)
					texturePrint(explo1, EXPLO_S, EXPLO_T, x, y-1, r);
					texturePrint(explo1, EXPLO_S, EXPLO_T, x, y-1, r+45);
				if(ex>=10)
					texturePrint(explo2, EXPLO_S, EXPLO_T, x, y-1, r);
					texturePrint(explo2, EXPLO_S, EXPLO_T, x, y-1, r+45);
			}
		}
	}
}

void draw_bullet(float x, float y, int v) {
	if(v) {
		texturePrint(bullet, BULLET_S, BULLET_T, x, y, 0);
	}
}

void draw_text(char *string,float x,float y) {  
	char *c;
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}





