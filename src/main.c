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
		
	Vec4* vs  = malloc(sizeof(Vec4) * vct);
	Vec4* vvs = malloc(sizeof(Vec4) * vct);
	Vec4* avs = malloc(sizeof(Vec4) * vct);
	for(int i = 0; i < vct; i++){
		vs [i] = (Vec4){0, rflt(), rflt(), rflt()};
		vvs[i] = (Vec4){0,      0,      0,      0};
		avs[i] = (Vec4){0,      0,      0,      0};
	}
	
	normalize(vs, vct);
	
	int* xs = malloc(sizeof(int) * vct);
	int* ys = malloc(sizeof(int) * vct);
	
	float rot  = 0.0;
	int cont = 1;
	while(cont){
		SDL_FillRect(screen, 0, 0);
		
		spring   (vs, vvs, avs, ns, 0.7, 0.1, vct);
		push     (vs, vvs, avs, 0.1, vct);
		normalize(vs, vct);
		
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
