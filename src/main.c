#include "SDL/SDL.h"
#include "defs.h"






static uint64_t rseed = 3518092371089;
float rflt(){
	rseed   = (rseed * 1349179819789) + 1471890674187;
	int rot =  rseed & 0x3f;
	rseed   = (rseed >> rot) | (rseed << (64-rot));
	int ret = rseed & 0xff;
	return (((float)ret) / 128.0) - 1.0;
}


void drawLine(uint32_t* ps, int x0, int y0, int x1, int y1){
	float dx = x0 - x1;
	float dy = y0 - y1;
	float  m = dy / dx;
	
	if(dx < 0){
		dx      = -dx;
		int tmp =  x0;
		x0      =  x1;
		x1      =  tmp;
	}
	if(dy < 0){
		dy      = -dy;
		int tmp =  y0;
		y0      =  y1;
		y1      =  tmp;
	}
	
	//printf("%f %f : %f\n", dx, dy, m);
	
	float carry = 0;
	if((m * m) < 1.0){
		for(int i = 0; i < dx; i++){
			int x =  x1 + i;
			int y =  y1 + carry;
			int p = (y * 512) + x;
			carry += m;
			ps[p] = 0xffffffff;
		}
	}else{
		for(int i = 0; i < dy; i++){
			int x =  x1 + carry;
			int y =  y1 + i;
			int p = (y * 512) + x;
			carry += m;
			ps[p] = 0xffffffff;
		}
	}
}







int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, 0);

		
	Vec4* vs = malloc(sizeof(Vec4) * 2048);
	for(int i = 0; i < 2048; i++){
		vs[i].w = 0.0;
		vs[i].x = rflt();
		vs[i].y = rflt();
		vs[i].z = rflt() - 2.5;
	}
	
	
	//int vct  = -1;
	//Node* ns = parse(argv[1], &vct);
	
	int cont = 5;
	while(cont){
		SDL_FillRect(screen, 0, 0);
		drawVecs(screen->pixels, vs, 2048);
		
		if(cont == 4){
			for(int i = 0; i < 2048; i++) vs[i] = warp(vs[i]);
			cont--;
		}
		if(cont == 2){
			normalize(vs, 2048);
			cont--;
		}
		
		//drawLine(screen->pixels, 0, 0, 511, 255);
		
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) cont--;
		}
		SDL_Flip (screen);
		SDL_Delay(10);
	}
	
	//SDL_Quit();	// Apparently this causes segfaults a lot? And it breaks valgrind when I try to figure otu why?
}
