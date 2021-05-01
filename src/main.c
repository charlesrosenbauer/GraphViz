#include "SDL/SDL.h"
#include "defs.h"
#include "math.h"





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
	
	float rot  = 0.0;
	int cont = 1;
	while(cont){
		SDL_FillRect(screen, 0, 0);
		
		optimize(vs, ns, 0.01, vct);
		
		drawGraph(screen->pixels, vs, xs, ys, ns, vct, 0);
		
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) cont = 0;
		}
		SDL_Flip (screen);
		SDL_Delay(10);
	}
	
	//SDL_Quit();	// Apparently this causes segfaults a lot? And it breaks valgrind when I try to figure otu why?
}
