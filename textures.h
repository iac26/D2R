/* DESTRUCTEUR 2 RADAR
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
 * Filename: textures.h
 * */

#ifndef TEXTURES_H
#define TEXTURES_H

typedef struct{
	float x;
	float y;
	float r;
	float g;
	float b;
}TEXTURE;

#define SQUARE_S 4
#define SQUARE_T GL_POLYGON
TEXTURE square[SQUARE_S]; 
TEXTURE fond[SQUARE_S];
TEXTURE road[SQUARE_S];
TEXTURE radar_box[SQUARE_S];
TEXTURE radar_mount[SQUARE_S];
TEXTURE radar_b_mount[SQUARE_S];
TEXTURE low_radar_mount[SQUARE_S];
TEXTURE low_radar_b_mount[SQUARE_S];
TEXTURE radar_cam[SQUARE_S];

#define CAR_S 10
#define CAR_T GL_POLYGON
TEXTURE car[CAR_S]; 

#define LAMP_S 4
#define LAMP_T GL_POLYGON
TEXTURE lamp1[LAMP_S];
TEXTURE lamp2[LAMP_S];

#define WINDOW_S 4
#define WINDOW_T GL_POLYGON
TEXTURE window1[WINDOW_S]; 
TEXTURE window2[WINDOW_S]; 

#define CIRCLE_S 8
#define CIRCLE_T GL_POLYGON
TEXTURE circle[CIRCLE_S];

#define CANNON_S 11
#define CANNON_T GL_POLYGON
TEXTURE cannon[CANNON_S]; 

#define BULLET_S 4
#define BULLET_T GL_POLYGON
TEXTURE bullet[BULLET_S]; 

#define STAR_S 8
#define STAR_T GL_POLYGON
TEXTURE star[STAR_S];

#define EXPLO_S 8
#define EXPLO_T GL_POLYGON
TEXTURE explo1[EXPLO_S];
TEXTURE explo2[EXPLO_S];

#endif
