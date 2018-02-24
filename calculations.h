/* DESTRUCTEUR 2 RADAR
 * Author: Iacopo Sprenger
 * Version: 1.0
 * Date: 24.02.2018
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

void calculate(void);
void collisions(void);
void update_bullet(BULLET* b); 
void fire_bullet(BULLET* b); 
void update_radar(RADAR* r);
void launch_radar(RADAR* r);
void autopilot(void);

#endif
