//game.c
#include <stdlib.h>
#include <stdio.h>
#include "calculations.h"
#include "constants.h"

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
