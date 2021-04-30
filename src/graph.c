#include "math.h"
#include "stdio.h"

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
	a.w = (a.w > 1.0)? 1.0 : (a.w < -1.0)? -1.0 : a.w;
	a.x = (a.x > 1.0)? 1.0 : (a.x < -1.0)? -1.0 : a.x;
	a.y = (a.y > 1.0)? 1.0 : (a.y < -1.0)? -1.0 : a.y;
	a.z = (a.z > 1.0)? 1.0 : (a.z < -1.0)? -1.0 : a.z;
	return a;
}

float normal(Vec4 a){
	float n = (a.w * a.w) + (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
	return sqrt(n);
}


void drawVecs(uint32_t* ps, Vec4* vs, int ct){
	float zpos = 1.5;
	for(int i = 0; i < ct; i++){
		int x = ((vs[i].x / (vs[i].z - zpos)) * 256) + 256;
		int y = ((vs[i].y / (vs[i].z - zpos)) * 256) + 256;
		if((x < 0) || (x > 511)) continue;
		if((y < 0) || (y > 511)) continue;
		ps[(y * 512) + x] = 0xffffffff;
	}
}


void projectVecs(Vec4* vs, int* xs, int* ys, int ct){
	float zpos = 1.5;
	for(int i = 0; i < ct; i++){
		xs[i] = ((vs[i].x / (vs[i].z - zpos)) * 256) + 256;
		ys[i] = ((vs[i].y / (vs[i].z - zpos)) * 256) + 256;
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
	for(int i = 1; i < ct; i++){
		max = maxV4(max, vs[i]);
		min = minV4(min, vs[i]);
	}
	Vec4 center = scaleV4(addV4(max, min), 0.5);
	float scale = 1.0 / normal (subV4(max, center));
	//printf("SCALE=%f\n", scale);
	//printf("CENTER=%f %f %f %f\n", center.w, center.x, center.y, center.z);
	for(int i = 0; i < ct; i++)
		vs[i] = scaleV4(subV4(vs[i], center), scale);
}




void drawLine(uint32_t* ps, int x0, int y0, int x1, int y1){
	int dx, dy;
	int temp, x, y;
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;
	temp = 2 * dy - dx;
	while(x < x1){
		if(temp < 0){
			temp = temp + 2 * dy;
		}else{
			y = y + 1;
			temp = temp + 2 * dy - 2 * dx;
		}
		int p = (y * 512) + x;
		x++;
		if((x < 0) || (x > 511)) continue;
		if((y < 0) || (y > 511)) continue;
		ps[p] = 0xffffff;
	}
}


void drawDot(uint32_t* ps, uint32_t color, int x, int y){
	for(int i = -2; i < 2; i++){
		for(int j = -2; j < 2; j++){
			int x_ = x + j;
			int y_ = y + i;
			if((y < 0) || (y > 511)) continue;
			if((x < 0) || (x > 511)) break;
			int p = (512 * y_) + x_;
			ps[p] = color;
		}
	}
}


void drawGraph(uint32_t* ps, Vec4* vs, int* xs, int* ys, Node* ns, int ct, int isDirected){
	float zpos = 1.5;
	// Project the points. Maybe this should be in a different function
	for(int i = 0; i < ct; i++){
		int x = ((vs[i].x / (vs[i].z - zpos)) * 256) + 256;
		int y = ((vs[i].y / (vs[i].z - zpos)) * 256) + 256;
		xs[i] = x;
		ys[i] = y;
	}
	
	for(int i = 0; i < ct; i++){
		int x = xs[i];
		int y = ys[i];
		drawDot(ps, 0xffff0000, x, y);
		for(int j  = 0; j < ns[i].edgect; j++){
			int v  = ns[i].edges[j];
			//if(!isDirected && (v <= i)) continue;
			int x_ = xs[v];
			int y_ = ys[v];
			//printf("L%i %i %i %i\n", x_, y_, x, y);
			drawLine(ps, x, y, x_, y_);
		}
	}
	//printf("\n");
}




