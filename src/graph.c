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

float length(Vec4 v){
	return sqrt((v.w * v.w) + (v.x * v.x) + (v.y * v.y) + (v.z * v.z));
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

Vec4 mulV4(Vec4 a, Vec4 b){
	a.w *= b.w;
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

float dist(Vec4 a, Vec4 b){
	return length(subV4(a, b));
}

float dotV4(Vec4 a, Vec4 b){
	a.w *= b.w;
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a.w + a.x + a.y + a.z;
}

Vec4 warp(Vec4 a){
	a = mulV4(a, addV4(a, (Vec4){0.0, 0.2, 0.3, 0.1}));
	a = mulV4(a, addV4(a, (Vec4){0.0, 0.3, 0.1, 0.2}));
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

void move(Vec4* vs, int vct, float scale){
	for(int i = 0; i < vct; i++){
		vs[i].x += rflt() * scale;
		vs[i].y += rflt() * scale;
		vs[i].z += rflt() * scale;
	}
	normalize(vs, vct);
}

void push(Vec4* vs, Vec4* vvs, Vec4* avs, float t, int vct){
	for(int i = 0; i < vct; i++)  vs[i] = addV4( vs[i], scaleV4(vvs[i], t));
	for(int i = 0; i < vct; i++) vvs[i] = addV4(vvs[i], scaleV4(avs[i], t));
}


Vec4 center(Vec4* vs, int ct){
	if(ct == 1) return vs[0];
	Vec4 sum = vs[0];
	for(int i = 1; i < ct; i++)
		sum = addV4(sum, vs[i]);
	return scaleV4(sum, 1.0 / ((float)ct));
}


void recenter(Vec4* vs, int ct){
	Vec4 cnt = center(vs, ct);
	for(int i = 0; i < ct; i++)
		vs[i] = subV4(vs[i], cnt);
}


void friction(Vec4* as, float f, int vct){
	for(int i = 0; i < vct; i++)
		as[i] = scaleV4(as[i], (1.0 - f));
}

void spring(Vec4* ps, Vec4* as, Node* ns, float k, float l, int vct){
	for(int i = 0; i < vct; i++){
		Vec4 pos = ps[i];
		for(int j = 0; j < ns[i].edgect; j++){
			int   n = ns[i].edges[j];
			float d = dist(pos, ps[n]);
			float f = k * (l - d);
			Vec4 dv = subV4(pos, ps[n]);
			Vec4 fv = scaleV4(dv, f);
			as[i]   = addV4(as[i], fv);
			as[n]   = subV4(as[n], fv);
		}
	}
}

void expand(Vec4* ps, Vec4* as, float f, int vct){
	Vec4  cnt = center(ps, vct);
	for(int i = 0; i < vct; i++){
		float d = sqrt (distSqr(cnt, ps[i]));
		as[i]   = addV4(as[i], scaleV4(cnt, f / (1.0 - d)));
	}
}


void optimize(Vec4* vs, Node* ns, float heat, int vct){
	Vec4 cnt = center(vs, vct);

	for(int i = 0; i < vct; i++){
		Vec4 best  = vs[i];
		float err  = 1e10;
		int rounds = 8;
		for(int k  = 0; k < rounds; k++){
			Vec4 test = vs[i];
			if(k != 0){
				test.x = vs[i].x + rflt() * heat;
				test.y = vs[i].y + rflt() * heat;
				test.z = vs[i].z + rflt() * heat;
			}
			
			float dev = 0;
			if(ns[i].edgect > 0){
				for(int j = 0; j < ns[i].edgect; j++)
					dev += distSqr(vs[i], vs[j]);
				dev /= ns[i].edgect;
			}
			
			float loc = dev - (0.3 * distSqr(test, cnt));
			printf("%f\n", loc);
			if(loc < err){
				best = test;
				err  = loc;
			}
		}
		vs[i] = best;
	}
	normalize(vs, vct);
}




int abs(int x){
	return (x < 0)? -x : x;
}


void drawLine(uint32_t* ps, int x0, int y0, int x1, int y1){
	int dx  = x1 - x0;
	int dy  = y1 - y0;
	float m = ((float)abs(dy)) / ((float)dx);
	
	int   x = x1;
	int   y = y1;
	float c = 0.0;
	if(abs(dx) > abs(dy)){	// from x1 -> x0
		int  ix = (dx < 0)? -1 : 1;
		float c = 0.0;
		if((dx ^ dy) > 0) m = -m;
		for(int i = 0; i < abs(dx); i++){
			x -= ix;
			c -= m;
			y  = y1 - c;
			int p = (y * 512) + x;
			if(!((x < 0) || (x > 511) || (y < 0) || (y > 511))) ps[p] = 0xffffffff;
		}
	}else{					// from y1 -> y0
		m = 1.0 / m;
		int iy = (dy < 0)? -1 : 1;
		for(int i = 0; i < abs(dy); i++){
			y -= iy;
			c += m;
			x  = x1 - c;
			int p = (y * 512) + x;
			if(!((x < 0) || (x > 511) || (y < 0) || (y > 511))) ps[p] = 0xffffffff;
		}
	}
}


void drawDot(uint32_t* ps, uint32_t color, int x, int y){
	for(int i = -2; i < 2; i++){
		for(int j = -2; j < 2; j++){
			int x_ = x + j;
			int y_ = y + i;
			if((y_ < 0) || (y_ > 511)) continue;
			if((x_ < 0) || (x_ > 511)) continue;
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
		for(int j  = 0; j < ns[i].edgect; j++){
			int v  = ns[i].edges[j];
			if(!isDirected && (v <= i)) continue;
			int x_ = xs[v];
			int y_ = ys[v];
			//printf("L%i %i %i %i\n", x_, y_, x, y);
			drawLine(ps, x, y, x_, y_);
		}
	}
	for(int i = 0; i < ct; i++)
		drawDot(ps, 0xffff0000, xs[i], ys[i]);
	//printf("\n");
}




