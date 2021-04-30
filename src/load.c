#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "defs.h"


void loadFile(char* fname, uint8_t** buffer, int* fsize){
  FILE*  pFile = fopen (fname,"r");
  size_t result;
  if (pFile == NULL){ printf("Cannot locate file.\n"); exit(1); }
  fseek (pFile , 0 , SEEK_END);
  *fsize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  *buffer = malloc (sizeof(uint8_t)*(*fsize));
  if (buffer == NULL) { printf("Memory error\n",stderr); exit(2); }

  // copy the file into the buffer:
  result = fread (*buffer,1,(*fsize),pFile);
  if (result != (*fsize)) { printf("Reading error\n",stderr); exit(3); }

  fclose(pFile);
}


Node* parse(char* fname, int* vct){
	uint8_t* buffer = NULL;
	int      fsize  = 0;
	int      lnct   = 1;
	int*     ixs    = NULL;
	{// Load files
		loadFile(fname, &buffer, &fsize);
		if(fsize < 1) return NULL;
	
		lnct      = 1;
		int lnix  = 0;
		for(int i = 0; i < fsize; i++){
			if(buffer[i] == '\n'){
				lnct++;
				lnix = i;
			}
		}
		lnct = 1;
		lnix = 0;
		ixs  = malloc(sizeof(int) * (lnct+1));
		for(int i = 0; i < lnct; i++) ixs[i] = 0;
		for(int i = 0; i < fsize; i++){
			if(buffer[i] == '\n'){
				ixs[lnct] = i+1;
				lnct++;
				lnix = i;
			}
		}
	}
	
	{// Parse files
		*vct = -1;
		for(int i = 0; i < lnct; i++){
			if(buffer[ixs[i]] ==  '\n') continue;	// Newline, skip
			if(buffer[ixs[i]] ==  '#' ) continue;	// comment, skip
			if(ixs[i]         == fsize) continue;	// EOF
			if(*vct != -1){
			
			}else{
				// First line
				if(0){
					fail:
						printf("Invalid file header\n");
						free(buffer);
						free(ixs);
						return NULL;
				}
				if(buffer[ixs[i]  ] != 'g') goto fail;
				if(buffer[ixs[i]+1] != 'r') goto fail;
				if(buffer[ixs[i]+2] != 'f') goto fail;
				if(buffer[ixs[i]+3] != ' ') goto fail;
				printf("Header pass!\n");
				*vct = 0;
			}
		}
	}
	
	free(buffer);
	free(ixs);
	return NULL;
}

