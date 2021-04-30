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
Vec4 addV4(Vec4, Vec4);
Vec4 subV4(Vec4, Vec4);
Vec4 scaleV4(Vec4, float);
Vec4 dotV4(Vec4, Vec4);

Vec4 warp(Vec4);

void normalize  (Vec4*, int);
void drawVecs   (uint32_t*, Vec4*, int);
void projectVecs(Vec4*, int*, int*, int);

#endif
