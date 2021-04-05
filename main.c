#include "SDL/SDL.h"


typedef struct{
	int* edges;
	int  edgect;
}Node;


int main(){
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, 0);
	
	int cont = 1;
	while(cont){
		
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) cont = 0;
		}
		SDL_Flip (screen);
		SDL_Delay(10);
	}
	
	SDL_Quit();
}
