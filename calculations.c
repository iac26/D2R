/* DESTRUCTEUR 2 RADAR
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
 * Filename: calculations.c
 * */
 
#include <stdlib.h>
#include <stdio.h>
#include "calculations.h"
#include "constants.h"
#include "game.h"

float aim = 20;
float cur_x = 0;
float cur_y = 0;
int aiming_up = 0;
int aiming_down = 0;
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
int ap = 0;
int power = 0; 
int freeze = 0;
int menu = 0;
int game_over = 0;
int menu_select = 0;
int menu_trigger = 0;
int active_mouse = 0;

void calculate() {
	if(!freeze) {
		tire_angle += TIRE_SPEED;
		if (tire_angle > 360.)
			tire_angle = 0.;
		if (aiming_up) {
			if (aim < 65){
				aim+=AIM_SPEED;
			}
		}
		if (aiming_down) {
			if (aim > 5) {
				aim-=AIM_SPEED;
			}
		}
		if (active_mouse) {
			float aim_x = -B_X0 + cur_x;
			float aim_y = B_Y0 - cur_y;
			aim = atanf(aim_y/aim_x)/0.0174;
			if (aim > 65){
				aim = 65;
			}
			if (aim < 5){
				aim = 5;
			}
		}
		if (fire == 1) { //fire a new bullet
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bullets[i].visible == 0){
					fire_bullet(&bullets[i]);
					break; 
				} 
			}
			fire = 0; 
		}
		if((global_step - last_sent) == next_send) { 
			last_sent = global_step;
			next_send = (rand()% 50 + 50)/difficulty;
			if (rad == 0) {
				rad = 1; 
			}
		}
		if (rad == 1){ // launch radar
			for (int i = 0; i < MAX_RADARS; i++) {
				if (radars[i].visible == 0){
					launch_radar(&radars[i]);
					break;
				} 
			}
			rad = 0; 
		}
		if (ap) {
			autopilot();
		}
		collisions();
		for (int i = 0; i < MAX_RADARS; i++) {
			update_radar(&radars[i]);
		}
		for (int i = 0; i < MAX_BULLETS; i++) {
			update_bullet(&bullets[i]);
		}
		if (global_step > 1000000) {
			last_sent = 0;
			global_step = 0;
		}
		global_step++;
		difficulty = (score / 30) + 1; 
		if (difficulty > MAX_DIFF)
			difficulty = MAX_DIFF; 
		if (score < 0)
			difficulty = 1;
		if (score > 500)
			difficulty = 10;
	}
	if(menu) {
		if(menu_trigger) {
			menu_trigger = 0;
			if(menu_select == 0) {
				freeze = 0;
				menu = 0;
			}
			if(menu_select == 1) {
				freeze = 0;
				menu = 0;
				reset();
			}
			if(menu_select == 2) {
				quit();
			}
		}
	}
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

void reset() {
	score = 0;
	for(int i = 0; i < MAX_RADARS; i++) {
		radars[i].visible = 0;
	}
	for(int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].visible = 0;
	}
}

void update_radar(RADAR* r) {
	if (r->visible == 1) {
		float t = (float) r->step / TIME_RATE;
		r->y = R_Y0;
		r->x = -R_SPEED*t + R_X0;
		r->step++;
		if (r->step > 300) 
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

float convert_x(int x) {
	return (((float) x / 900)*15) - 3; 
}

float convert_y(int y) {
	return (((float) y / 500)*8.34) - 2.78; 
}

float t_aim = 20;
int rs;
int cd[MAX_RADARS] = {0};
int rp = 0;
int counter = 0;
void autopilot(void) {
	if (rp == 0) {
		for (int i = 0; i < MAX_RADARS; i++) {
			cd[i]--;
			if ((radars[i].x < 8)&&(radars[i].intact == 1)&&(radars[i].x > 7.5)&&
				(cd[i] < 0)&&(radars[i].visible == 1)) {
				rs = i;
				rp = 1;
			}
		}
	}else {
		if (radars[rs].low == 1) {
			t_aim = 17; 
		} else {
			t_aim = 30; 
		}
		if (aim < t_aim) {
			aim += AIM_SPEED;
		}
		if (aim > t_aim) {
			aim -= AIM_SPEED;
		}
		if ((aim > t_aim-2)&&(aim < t_aim+2)) {
			cd[rs] = 20;
			rp = 0;
			if (power) {
				fire = 1;
			}else{
				if (counter != 5){
					fire = 1;
					counter++;
				}else {
					counter = 0;
				}
			}
		}
	}
}
