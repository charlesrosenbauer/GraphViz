#include "SDL/SDL.h"


typedef struct{
	int* edges;
	int  edgect;
}Node;

typedef struct{
	float w, x, y, z;
}Vec4;


void drawVecs(uint32_t* ps, Vec4* vs, int ct){
	for(int i = 0; i < ct; i++){
		int x = ((vs[i].x / vs[i].z) * 256) + 256;
		int y = ((vs[i].y / vs[i].z) * 256) + 256;
		x = (x < 0)? 0 : (x > 511)? 511 : x;
		y = (y < 0)? 0 : (y > 511)? 511 : y;
		ps[(y * 512) + x] = 0xffffffff;
	}
}

static uint64_t rseed = 3518092371089;
float rflt(){
	rseed   = (rseed * 1349179819789) + 1471890674187;
	int rot =  rseed & 0x3f;
	rseed   = (rseed >> rot) | (rseed << (64-rot));
	int ret = rseed & 0xff;
	return (((float)ret) / 128.0) - 1.0;
}


int main(){
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, 0);
	
	Vec4* vs = malloc(sizeof(Vec4) * 2048);
	for(int i = 0; i < 2048; i++){
		vs[i].w = rflt();
		vs[i].x = rflt();
		vs[i].y = rflt();
		vs[i].z = rflt() - 2.5;
	}
	
	int cont = 1;
	while(cont){
		SDL_FillRect(screen, 0, 0);
		drawVecs(screen->pixels, vs, 2048);
		
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) cont = 0;
		}
		SDL_Flip (screen);
		SDL_Delay(10);
	}
	
	SDL_Quit();
}
