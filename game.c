/* DESTRUCTEUR 2 RADAR
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
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

void affichage(void);
void keyboard(char key);
void keyboard_2(char key);
void mouse(int button, int state, int x, int y);
void mouse_hover(int x, int y);
void reshape(int w, int h);
void texturePrint(TEXTURE* tex, int size, int type, float x, float y, float r);
void draw_car(float x, float y);
void draw_radar(float x, float y, int v, int e, int ex, int h);
void draw_bullet(float x, float y, int v);
void draw_text(char *string,float x,float y, float r, float g, float b);

int main(int argc, char **argv) {
	srand(time(NULL));
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(900,500);
	glutCreateWindow("Destructeur 2 Radar"); 
	glutDisplayFunc(affichage);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_2);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse_hover);
	glutReshapeFunc(reshape);
	glutTimerFunc(FLEN, calculate, 0);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutMainLoop();
}

void affichage(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_POLYGON_SMOOTH);
	glOrtho(GAUCHE, DROITE, BAS, HAUT,-1,1); 
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
	sprintf(str, "CASH: %d $", score);
	float shift;
	if ((score > 9)||(score < 0))
		shift = 0.2;
	if ((score > 99)||(score < -9))
		shift = 0.4;
	if ((score > 999)||(score < -99))
		shift = 0.6;
	draw_text(str, 10-shift, -2.3, 1.0, 1.0, 1.0);
	if(menu) {
		if(menu_select == 0) {
			draw_text("RESUME", 3.5, 0.0, 0.0, 0.0, 0.0);
		}else{
			draw_text("RESUME", 3.5, 0.0, 1.0, 1.0, 1.0);
		}
		if(menu_select == 1) {
			draw_text("RESTART", 3.5, 0.5, 0.0, 0.0, 0.0);
		}else{
			draw_text("RESTART", 3.5, 0.5, 1.0, 1.0, 1.0);
		}
		if(menu_select == 2) {
			draw_text("QUIT", 3.5, 1.0, 0.0, 0.0, 0.0);
		}else{
			draw_text("QUIT", 3.5, 1.0, 1.0, 1.0, 1.0);
		}
	}
	if(ap) {
		if(power) {
			draw_text("P", 11.7, 5.5, 0.5, 1.0, 0.5);
		}else{
			draw_text("P", 11.7, 5.5, 1.0, 0.5, 0.5);
		}
	}
	
	glutSwapBuffers();
}



void reshape(int w, int h) {
	glutReshapeWindow(900, 500);
}

void keyboard(char key) {
	switch(key) {
		case 'w':
			aiming_up = 1;
			if (menu_select > MENU_LOW)
				menu_select -= 1;
			break;
		case 's':
			aiming_down = 1;
			if (menu_select < MENU_HIGH)
				menu_select += 1;
			break;
		case ' ':
			if (!freeze) {
				if (fire == 0)
					fire = 1; 
			} else {
				menu_trigger = 1;
			}
			break;
		case '\r':
			if (freeze) {
				menu_trigger = 1;
			}
			break;
		case 'p':
			if (ap == 0) {
				ap = 1; 
				active_mouse = 0;
			}else {
				ap = 0;
			}
			break;
		case 'o':
			if (power == 0) {
				power = 1; 
			}else {
				power = 0;
			}
			break;
		case 'm':
			if (active_mouse == 0) {
				active_mouse = 1; 
			}else {
				active_mouse = 0;
			}
			break;
		case 27:
			if (freeze == 0) {
				freeze = 1;
				menu = 1;
				menu_select = 0;
			}else {
				freeze = 0;
				menu = 0;
			}
			break;
		case 'q':
			quit();
			break;
	}
}

void keyboard_2(char key) {
	switch(key) {
		case 'w':
			aiming_up = 0;
			break;
		case 's':
			aiming_down = 0;
			break;
		}
}

void mouse(int button, int state, int x, int y) {
	if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)) {
		if (!freeze) {
			if (fire == 0)
					fire = 1; 
		} else {
			menu_trigger = 1;
		}
	}
	if((button == GLUT_RIGHT_BUTTON)&&(state == GLUT_DOWN)) {
		if (active_mouse == 0) {
			active_mouse = 1; 
		}else {
			active_mouse = 0;
		}
	}
}

void mouse_hover(int x, int y) {
	if(menu){
		float fx = convert_x(x);
		float fy = convert_y(y);
		if ((fx < 5.5)&&(fx > 3.5)) {
			if ((fy < 0.25)&&(fy > -0.25))
				menu_select = 0;
			if ((fy < 0.75)&&(fy > 0.25))
				menu_select = 1;
			if ((fy < 1.25)&&(fy > 0.75))
				menu_select = 2;
		}
	}else{
		cur_x = convert_x(x);
		cur_y = convert_y(y);
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

void draw_text(char *string, float x, float y, float r, float g, float b) {  
	char *c;
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void quit(void) {
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutDestroyWindow(1);
	exit(0);
}





