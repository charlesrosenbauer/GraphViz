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

void move(Vec4* vs, int vct, float scale){
	for(int i = 0; i < vct; i++){
		vs[i].x += rflt() * scale;
		vs[i].y += rflt() * scale;
		vs[i].z += rflt() * scale;
	}
	normalize(vs, vct);
} 










int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, 0);


	int vct  = -1;
	Node* ns = parse(argv[1], &vct);
	printf("VERTS=%i\n", vct);
	for(int i = 0; i < vct; i++){
		printf("%i | ", i);
		for(int j = 0; j < ns[i].edgect; j++) printf("%i ", ns[i].edges[j]);
		printf("\n");
	}
		
	Vec4* vs = malloc(sizeof(Vec4) * vct);
	for(int i = 0; i < vct; i++){
		vs[i].w = 0.0;
		vs[i].x = rflt();
		vs[i].y = rflt();
		vs[i].z = rflt();
	}
	
	normalize(vs, vct);
	
	int* xs = malloc(sizeof(int) * vct);
	int* ys = malloc(sizeof(int) * vct);
	
	int cont = 1;
	while(cont){
		SDL_FillRect(screen, 0, 0);
		
		move(vs, vct, 0.02);
		normalize(vs, vct);
		//drawVecs(screen->pixels, vs, vct);
		
		drawGraph(screen->pixels, vs, xs, ys, ns, vct, 0);
		
		//drawLine(screen->pixels, 0, 0, 511, 255);
		
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) cont = 0;
		}
		SDL_Flip (screen);
		SDL_Delay(10);
	}
	
	//SDL_Quit();	// Apparently this causes segfaults a lot? And it breaks valgrind when I try to figure otu why?
}
