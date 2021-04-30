#include "math.h"

#include "defs.h"



float distSqr(Vec4 a, Vec4 b){
	float dw = a.w - b.w;
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return (dw * dw) + (dx * dx) + (dy * dy) + (dz * dz);
}

Vec4 addV4(Vec4 a, Vec4 b){
	a.w += b.w;
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vec4 subV4(Vec4 a, Vec4 b){
	a.w -= b.w;
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vec4 scaleV4(Vec4 a, float n){
	a.w *= n;
	a.x *= n;
	a.y *= n;
	a.z *= n;
	return a;
}

Vec4 dotV4(Vec4 a, Vec4 b){
	a.w *= b.w;
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

Vec4 warp(Vec4 a){
	a = dotV4(a, addV4(a, (Vec4){0.0, 0.2, 0.3, 0.1}));
	a = dotV4(a, addV4(a, (Vec4){0.0, 0.3, 0.1, 0.2}));
	return a;
}

float normal(Vec4 a){
	float n = (a.w * a.w) + (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
	return sqrt(n);
}


void drawVecs(uint32_t* ps, Vec4* vs, int ct){
	for(int i = 0; i < ct; i++){
		int x = ((vs[i].x / vs[i].z) * 256) + 256;
		int y = ((vs[i].y / vs[i].z) * 256) + 256;
		if((x < 0) || (x > 511)) continue;
		if((y < 0) || (y > 511)) continue;
		ps[(y * 512) + x] = 0xffffffff;
	}
}


void projectVecs(Vec4* vs, int* xs, int* ys, int ct){
	for(int i = 0; i < ct; i++){
		xs[i] = ((vs[i].x / vs[i].z) * 256) + 256;
		ys[i] = ((vs[i].y / vs[i].z) * 256) + 256;
	}
}

Vec4 maxV4(Vec4 a, Vec4 b){
	Vec4 ret;
	ret.w = (a.w > b.w)? a.w : b.w;
	ret.x = (a.x > b.x)? a.x : b.x;
	ret.y = (a.y > b.y)? a.y : b.y;
	ret.z = (a.z > b.z)? a.z : b.z;
	return ret;
}

Vec4 minV4(Vec4 a, Vec4 b){
	Vec4 ret;
	ret.w = (a.w < b.w)? a.w : b.w;
	ret.x = (a.x < b.x)? a.x : b.x;
	ret.y = (a.y < b.y)? a.y : b.y;
	ret.z = (a.z < b.z)? a.z : b.z;
	return ret;
}

void normalize(Vec4* vs, int ct){
	if(ct < 1) return;
	Vec4 max = vs[0], min = vs[0];
	for(int i = 0; i < ct; i++){
		max = maxV4(max, vs[i]);
		min = minV4(min, vs[i]);
	}
	Vec4 center = scaleV4(addV4(max, min), 0.5);
	float scale = normal (subV4(max, center));
	for(int i = 0; i < ct; i++)
		vs[i] = scaleV4(subV4(vs[i], center), scale);
}

