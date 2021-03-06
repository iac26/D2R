/* DESTRUCTEUR 2 RADAR
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
 * Filename: calculations.h
 * */
#ifndef CALCULATIONS_H
#define CALCULATIONS_H

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

float aim;
int aiming_up;
int aiming_down;
float tire_angle;
float cur_x;
float cur_y;
int fire; 
int flash;
int score; 
int global_step;
int last_sent; 
int next_send; 
int difficulty;
BULLET bullets[];
RADAR radars[]; 
int rad;
int ap;
int power;
int freeze;
int menu;
int game_over;
int menu_select;
int menu_trigger;
int active_mouse;

void calculate(void);
void collisions(void);
void update_bullet(BULLET* b); 
void fire_bullet(BULLET* b); 
void update_radar(RADAR* r);
void launch_radar(RADAR* r);
void autopilot(void);
void reset(void);

float convert_x(int x);
float convert_y(int y);

#endif
