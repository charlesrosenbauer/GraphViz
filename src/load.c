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


void skipWhite(uint8_t* buffer, int* ix, int end){
	for(int i = *ix; i < end; i++){
		char c = buffer[i];
		*ix = i;
		if((c != ' ') || (c != '\t')) break;
	}
}


int parseInt(uint8_t* buffer, int* ix, int end){
	int n = 0;
	for(int i = *ix; i < end; i++){
		char c = buffer[i];
		if((c > '9') || (c < '0')) break;
		n *= 10;
		n += (c - '0');
		*ix = i;
	}
	return n;
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
		ixs  = malloc(sizeof(int) * (lnct+5));
		for(int i = 0; i < lnct; i++) ixs[i] = 0;
		for(int i = 0; i < fsize; i++){
			if(buffer[i] == '\n'){
				ixs[lnct] = i+1;
				lnct++;
				lnix = i;
			}
		}
	}
	
	Node* ret;
	
	{// Parse files
		*vct    = -1;
		for(int i = 0; i < lnct; i++){
			if(buffer[ixs[i]] ==  '\n') continue;	// Newline, skip
			if(buffer[ixs[i]] ==  '#' ) continue;	// comment, skip
			if(ixs[i]         == fsize) continue;	// EOF
			if(*vct != -1){
				if(0){
					failLine:
						printf("Invalid vertex format at line %i\n", i+1);
						free(buffer);
						free(ixs);
						return NULL;
				}
				int ix   = ixs[i];
				int init = ix;
				int end  = (i+1 >= lnct)? fsize : ixs[i+1];
				int n    = parseInt(buffer, &ix, end);
				if (n ==   0) goto failLine;
				if (n > *vct) goto failLine;
				skipWhite(buffer, &ix, end);
				if(buffer[ix+2] != ':') goto failLine;
				ix += 1;
				skipWhite(buffer, &ix, end);
				int last = ix-1;
				ret[n].edges = malloc(sizeof(int) * (end-ix));
				
				int edgeix = 0;
				for(int j  = ix; j < end; j++){
					last   = ix;
					int v  = parseInt(buffer, &j, end);
					if(v == 0) continue;
					ret[n].edges[edgeix] = v;
					edgeix++;
					skipWhite(buffer, &j, end);
				}
				for(int j = 0; j < edgeix; j++) ret[n].edges[j]--;
				ret[n].edgect = edgeix;
				
				
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
				int ix  = ixs[i]+4;
				int end = (i+1 >= lnct)? fsize : ixs[i+1];
				skipWhite(buffer, &ix, end);
				int n   = parseInt(buffer, &ix, end);
				ret = malloc(sizeof(Node) * (n+1));
				*vct = n;
			}
		}
	}
	
	free(buffer);
	free(ixs);
	return &ret[1];
}

