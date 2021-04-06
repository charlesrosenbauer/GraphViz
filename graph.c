#include "defs.h"



float distSqr(Vec4 a, Vec4 b){
	float dw = a.w - b.w;
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return (dw * dw) + (dx * dx) + (dy * dy) + (dz * dz);
}




