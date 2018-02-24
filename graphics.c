//graphics.c
//NOT USED ANYMORE

#include <stdlib.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "game.h"
#include "textures.h"
#define HAUT -5
#define BAS 10
#define DROITE 12
#define GAUCHE -3
#define FLEN 20
#define TIRE_SPEED 10.
#define AIM_SPEED 5.
#define BULLET_SPEED 8.
#define B_X0 -0.3
#define B_Y0 4.05
#define R_X0 13.
#define R_Y0 4.3
#define MAX_BULLETS 4
#define MAX_RADARS 10
#define TIME_RATE 50.
#define R_SPEED 5.

float ratio = 1.; 

typedef struct {
	float x;
	float y;
	int visible;
	int intact;
	int active;
	int e_step;
	int step;
	int low; 
}RADAR;

typedef struct {
	float x;
	float y;
	float vx;
	float vy; 
	int visible;
	int step; 
}BULLET;

void affichage(void);
void calculate(void);
void collisions(void);
void update_bullet(BULLET* b); 
void fire_bullet(BULLET* b); 
void update_radar(RADAR* r);
void launch_radar(RADAR* r);
void keyboard(char key);
void reshape(int w, int h);
void texturePrint(TEXTURE* tex, int size, int type, float x, float y, float r);
void draw_car(float x, float y);
void draw_radar(float x, float y, int v, int e, int ex, int h);
void draw_bullet(float x, float y, int v);
void draw_text(char *string,float x,float y);
float aim = 20;
float tire_angle = 0;
int fire = 0; 
int flash = 0;
int score = 0; 
int global_step = 0;
int last_sent = 0; 
int next_send = 0; 
int difficulty = 1;
BULLET bullets[MAX_BULLETS];
RADAR radars[MAX_RADARS]; 

int rad = 0;


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
	glutTimerFunc(FLEN, calculate, 0);
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

void calculate() {
	int i;
	tire_angle += TIRE_SPEED;
	if (tire_angle > 360.)
		tire_angle = 0.;
	if (fire == 1) { //fire a new bullet
		for (i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i].visible == 0){
				fire_bullet(&bullets[i]);
				break; 
			} 
		}
		fire = 0; 
	}
	
	if((global_step - last_sent) == next_send) { 
		last_sent = global_step;
		next_send = (rand()% 100 + 50)/difficulty;
		if (rad == 0) {
			rad = 1; 
		}
	}
	if (rad == 1){ // launch radar
		for (i = 0; i < MAX_RADARS; i++) {
			if (radars[i].visible == 0){
				launch_radar(&radars[i]);
				break;
			} 
		}
		rad = 0; 
	}
	collisions();
	for (i = 0; i < MAX_RADARS; i++) {
		update_radar(&radars[i]);
	}
	for (i = 0; i < MAX_BULLETS; i++) {
		update_bullet(&bullets[i]);
	}
	if (global_step > 1000000) {
		last_sent = 0;
		global_step = 0;
	}
	global_step++;
	difficulty = (score / 10) + 1; 
	if (score < 0)
		difficulty = 1;
	glutPostRedisplay();
	glutTimerFunc(FLEN, calculate, 0);
}

void fire_bullet(BULLET* b) {
	b->x = B_X0;
	b->y = B_Y0;
	b->vx = cos(-aim*0.0174)*BULLET_SPEED;
	b->vy = sin(-aim*0.0174)*BULLET_SPEED;
	b->visible = 1;
	b->step = 0;
}

void update_bullet(BULLET* b) {
	if (b->visible == 1) {
		float t = (float) b->step / TIME_RATE;
		b->x = b->vx * t + B_X0;
		b->y = 2.*t*t + b->vy*t + B_Y0; 
		b->step++; 
		if (b->step > 200) 
			b->visible = 0;
	}
}

void launch_radar(RADAR* r) {
	r->x = R_X0;
	r->y = R_Y0;
	r->visible = 1;
	r->intact = 1;
	r->active = 1;
	r->step = 0;
	r->e_step = 0;
	r->low = rand() % 2; 
}

void update_radar(RADAR* r) {
	if (r->visible == 1) {
		float t = (float) r->step / TIME_RATE;
		r->y = R_Y0;
		r->x = -R_SPEED*t + R_X0;
		r->step++;
		if (r->step > 500) 
			r->visible = 0;
		if(r->e_step){
			r->e_step++;
			if(r->e_step > 10) {
				r->e_step = 0;
			}
		}
	}
}

void collisions(void) {
	for(int i = 0; i < MAX_RADARS; i++) {
		if((radars[i].x < 1.)&&(radars[i].intact == 1)&&(radars[i].visible == 1)&&
			(radars[i].active == 1)) {
			radars[i].active = 0;
			if (radars[i].low == 0){
				flash = 1;
			}
			if (radars[i].low == 1){
				flash = 2;
			}
			score -= 5;
		}
		for (int j = 0; j < MAX_BULLETS; j++){
			if(bullets[j].y > 5) {
				bullets[j].visible = 0;
			}
			if((bullets[j].visible == 1)&&(radars[i].intact == 1)) {
				if(	(bullets[j].x < radars[i].x+0.3)&&(bullets[j].x > radars[i].x-0.3)&&
					(bullets[j].y < radars[i].y-1.7)&&(bullets[j].y > radars[i].y-2.3)&&
					(radars[i].low == 0)) {
					score += 1;
					radars[i].intact = 0;
					radars[i].e_step = 1;
					bullets[j].visible = 0;
				}
				if(	(bullets[j].x < radars[i].x+0.3)&&(bullets[j].x > radars[i].x-0.3)&&
					(bullets[j].y < radars[i].y-0.7)&&(bullets[j].y > radars[i].y-1.3)&&
					(radars[i].low == 1)) {
					score += 1;
					radars[i].intact = 0;
					radars[i].e_step = 1;
					bullets[j].visible = 0;
				}
				
			}
		}
		
	}
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
			if (aim > 2)
				aim -= AIM_SPEED;
			break;
		
		case ' ':
			if (fire == 0)
				fire = 1; 
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





