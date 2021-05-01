#ifndef __DEFS_HEADER__
#define __DEFS_HEADER__


#include "stdint.h"



typedef struct{
	int* edges;
	int  edgect;
}Node;

typedef struct{
	float w, x, y, z;
}Vec4;



Node* parse(char*, int*);


// Math
Vec4  addV4(Vec4, Vec4);
Vec4  subV4(Vec4, Vec4);
Vec4  scaleV4(Vec4, float);
Vec4  mulV4(Vec4, Vec4);
float dotV4(Vec4, Vec4);

Vec4  warp(Vec4);

void  normalize  (Vec4*, int);
void  move       (Vec4*, int, float);

// Optimization and Physics
void  push       (Vec4*, Vec4*, Vec4*, float, int);
void  spring     (Vec4*, Vec4*, Vec4*, Node*, float, float, int);
void  optimize   (Vec4*, Node*, float, int);

// Rendering
void  drawVecs   (uint32_t*, Vec4*, int);
void  projectVecs(Vec4*, int*, int*, int);
void  drawLine   (uint32_t*, int, int, int, int);
void  drawGraph  (uint32_t*, Vec4*, int*, int*, Node*, int, int);

// Rng
float rflt();


#endif
