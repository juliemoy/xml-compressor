#include "xpm.h"

node *z, *head;

/*Function: decompress
 * ----------------------
 * decompresses the bitfile and recreates the .xpm
 * file using the information pass through the codefile
 * 
 * may be run separately from compress.c and main
 */
void decompress()
{
	get_code_info();	
	make_tree();
	decode();
	deallocate(head);
	printf("program complete!\n");
}
/* Function: get_code_info
 * ----------------------------
 * gets the info from the codefile 
 * and puts it into the global arrays and variables
 */
void get_code_info()
{
	fp = fopen(codefile, "r");
	fscanf(fp, "%d %d %d %d\n", &width, &length, &colors, &symbol);
	get_arrays();
	fclose(fp);
}

/* Function: get_arrays
 * --------------------------
 * gets the arrays from the codefile and puts the
 * info into the global arrays
 */
void get_arrays()
{
	int i, j;
	symbols[1][0] = ' '; fscanf(fp, "%11d%10d\n", &code[0], &code_len[0]);
	for(i=1; i<colors; i++)
	{
		for(j=0; j<symbol; j++) symbols[i+1][j] = fgetc(fp);
		fscanf(fp, "%10d%10d\n", &code[i], &code_len[i]);
	}
}

/* Function: make_tree()
 * ----------------------------
 * creates a binary tree used for searching for the correct
 * symbol from the bitfile
 */
void make_tree()
{
	int i, j, bit, b;	//b represents the code length
	
	//initialize nodes
	z = (node*)malloc(sizeof(node*));
	head = (node*)malloc(sizeof(node*));
	head->l = z;
	head->r = z;
	head->key = 0;
	
	node *prev, *current;
	
	for(i=0; i<colors; i++)
	{
		current = head;
		b = code_len[i];
		
		while(b>0)
		{
			prev = current;
			if((bit = bits(code[i], --b, 1)) != 0) current = current->r;
			else current = current->l;
			
			if(current == z)
			{
				current = (node*)malloc(sizeof(node*));
				current->l = z; current->r = z; current->key = 0;
				
				if(bit !=0) prev->r = current;
				else prev->l = current;
			}
		}
		current->key = i+1;
	}	
}


/* Function: decode
 * -------------------
 * decodes the binary file by searching the tree 
 * and outputting the resulting code
 */
void decode()
{
	fp = fopen(bitfile, "rb");
	FILE* fw = fopen(newfile, "w");
	int bit, i, j;
	node* current = head;
	
	i=0;
	while(i<length)
	{
		j=0;
		while(j<width)
		{
			fread(&bit, sizeof(bit), 1, fp);
			//fscanf(fp, "%1d", &bit);
			if(bit) current = current->r;
			else current = current->l;
			
			if(current->key)
			{
				fprintf(fw, "%s", symbols[current->key]); 
				current = head; 
				j++;
			}
		}
		fprintf(fw, "\n");
		i++;
	}
	fclose(fp);
	fclose(fw);
}

/* Function: deallocate
 * --------------------------
 * deallocates the binary tree that was created for searching 
 * using recursion
 * 
 * x: node to start at to begin deallocation
 */
void deallocate(node* x)
{
	if(x == z) return;
	
	deallocate(x->l);
	deallocate(x->r);
	
	free(x);
}





