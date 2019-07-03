/*
 * This program uses the files xpm.h, compress.c and decompress.h as well.
 * To change the .xpm that is being encoded you can change the 
 * filename in the xpm.h headerfile.  
 * 
 * I included a makefile ($make xpm_compresser and $./xpm_compresser) 
 * to compile and run the programs. I kept having to compile the header 
 * file separately first for a reason I couldn't figure out.
 */

#include "xpm.h"

int main()
{
	initialize();
	fp = fopen(filename, "r");
	if(fp == NULL) {printf("ERROR openeing file\n"); exit(-1);}
	get_info();
	get_count();
	fclose(fp);
	get_heap();
	construct();
	encode();
	write();
	decompress();
}

void initialize()
{	
	fp = NULL;
	len = 100;
	
	int i;
	for(i=0; i<SIZE; i++)
		{count[i] = 0; heap[i] = 0; dad[i] = 0;}
}
	
	
