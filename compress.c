#include "xpm.h"

/* function: get_info()
 * ---------------------
 * gets the info from the .xpm including and calls get_symbols() 
 * to fill symbols array (all global variables)
 * 
 * width: num pixels wide
 * length: num pixels long 
 * colors: number of colors represented as symbols
 * symbol: number of chars representing each color
 */
void get_info()
{
	getline(&line, &len, fp);
	getline(&line, &len, fp);
	fscanf(fp, "\"%d %d %d %d\",\n", &width, &length, &colors, &symbol);
	get_symbols();
}

/* Function: get_symbols()
 * ---------------------------
 * fills global symbols array with symbols for each color,
 * gets info from .xpm file that has already been opened
 */
void get_symbols()
{
	int i=0, j;
	char c;
	char str[SYMBOL_LENGTH];
	while(i<colors)
	{
		j=0;
		fscanf(fp, "\"%c", &symbols[i][j]);
		for(j=1;j<symbol; j++)
			fscanf(fp, "%c", &symbols[i][j]);
		while(fgetc(fp)!= '\n');
		i++;
	}
}

/* Function: get_count()
 * -------------------------
 * Fills count array.  Each index of count[i] represents the number of 
 * times that symbos[i](the symbol at the corresponding index) occurs
 * in the .xpm file
 */ 
void get_count()
{
	int i, j, k;
	char s[SYMBOL_LENGTH];
	for(i=0; i<length; i++)
	{
		fgetc(fp); //get "
		for(j=1; j<=width; j++)
		{
			for(k=0; k<symbol; k++) 
				s[k] = fgetc(fp);
			count[symbol_search(s)]++;
		}
		while(fgetc(fp)!= '\n');
	}
}

/* Function: symbol_search
 * ---------------------------
 * searches the symbols array for the matching symbol that 
 * was passed to it
 * 
 * char* str: symbol to be searched for
 * 
 * return: int index where symbol is located in symbols array,
 * 	-1 if no symbol found
 */
int symbol_search(char* str)
{
	//printf("searching %s\t", str);
	int i,j, match; 
	char* ptr = str;
	for(i=0; i<colors; i++)
	{
		match = TRUE;
		ptr = str;
		for(j=0; j<symbol; j++)
		{
			if(symbols[i][j] != *ptr) match = FALSE;
			ptr++;
		}
		if(match == TRUE)return i;
	}
	printf("No symbol found!\n");
	return -1;
} 

/*Function: get_heap
 * -----------------------
 * Fills heap array with indexes of where the 
 * count does not equal zero, then calls downheap 
 * so that heap indexes with their corresponding counts
 * fulfill the heap property
 */
void get_heap()
{
	int i, k;
	for(i=0, heap_size=0; i<colors; i++)
		if(count[i])heap[++heap_size] = i;
	const_heap = heap_size;
	for(k = heap_size/2; k > 0; k--) downheap(k);
}

/* Function: downheap
 * -------------------------
 * creates a heap of the count array with the smalles at the
 * beginning using the heap array as indexes
 * 
 * int: k is the index to be heaped
 */
void downheap(int k)
{
	int j, v, h;
	h = heap[k];
	v = count[heap[k]];
	
	while(k <= heap_size/2)
	{
		j = (2*k);
		if(j < heap_size && count[heap[j]] > count[heap[j+1]]) j++;
		if(v < count[heap[j]]) break;
		
		heap[k] = heap[j]; 
		k = j;
	}
	heap[k] = h;
}

/* Function: construct
 * ----------------------
 * Huffman algorithm. Essentially creates the trie to be used for 
 * encoding.  Accomplished by taking the two smallest off the top 
 * of the heap, adding their counts and reinserting the new number onto
 * the heap.  Left and right nodes of the heap kept track of 
 * by the dad array which records the parent of the current node
 */
void construct()
{
	int t;
	while(heap_size > 1)
	{
		t = heap[1]; heap[1] = heap[heap_size--];
		downheap(1);
		count[colors+heap_size] = count[heap[1]]+count[t];
		dad[t] = colors+heap_size; dad[heap[1]] = -colors-heap_size;
		heap[1] = colors+heap_size; downheap(1);
	}
	dad[colors+heap_size] = 0;
}

/* Function: encode
 * ---------------------
 * Using the trie, creates the code to be used to endcode
 * the symbols.  Assigns the code(number to be produced in binary)
 * and the code_len(length of binary digits for each number in code[])
 * arrays for each index of the symbols array
 */
void encode()
{
	int i,j,k,t,x;
	for(k=0; k<colors; k++)
	{
		if(!count[k]){code[k] = 0; code_len[k] = 0;}
		else
		{
			i=0; j=1; t=dad[k]; x=0;
			while(t)
			{
				if(t<0) {x+=j; t = -t;}
				t = dad[t]; j+=j; i++;
			}
			code[k] = x; code_len[k] = i;
		}
	}
}

/* Function: bits
 * -------------------
 * returns the bit in the location that is requested
 * 
 * x: number of the code to be determined
 * k: place in the binary number to start
 * j: number of digits to the right to return
 * 
 * return: int bits located at specified location
 */
int bits(int x, int k, int j)
{ return (x >> k) & ~(~0<<j);}

/* Function: write
 * --------------------
 * writes the binary code to a file by going through the .xpm file,
 * at each symbol finding the corresponding binary code and writing it
 * to the bitfile.  Calls write_code() function to write code 
 * to the codefile
 */
void write()
{
	int i, j, k, l, s, bit;
	FILE* fw = NULL, *fr = NULL;
	char str[SYMBOL_LENGTH];
	
	fw = fopen(bitfile, "wb");
	fr = fopen(filename, "r");
	//printf("file pointer: %ld\n", ftell(fr));
	
	//move to where picture starts
	for(i=0; i<colors+3; i++) getline(&line, &len, fr);
	
	for(i=0; i<length; i++)
	{
		fgetc(fr);	//get "
		for(j=0; j<width; j++)
		{
			for(k=0; k<symbol; k++) str[k] = fgetc(fr); //get symbol
			s = symbol_search(str);						//get symbol location
			for(l=code_len[s]; l>0; l--)				//print binary code
			{
				bit = bits(code[s], l-1, 1);
				fwrite(&bit, sizeof(bit), 1, fw);	
				//fprintf(fw, "%1d", bits(code[s], l-1, 1));
			}
		}
		while(fgetc(fr) != '\n');	//move pointer fr to end of line
	}
	fclose(fw);
	fclose(fr);
	write_code();

}

/* Function: write_code
 * ------------------------
 * writes the information needed to decompress the binary 
 * file to a codefile.  Info includes width, length, colors, 
 * symbol, and the symbols, code, and code_len arrays
 */
void write_code()
{
	int i,j;
	FILE* fw = fopen(codefile, "w");
	fprintf(fw, "%d %d %d %d\n", width, length, colors, symbol);
	for(i=0; i<colors; i++)
	{
		for(j=0; j<symbol; j++) fprintf(fw, "%c", symbols[i][j]);
		fprintf(fw, "%10d%10d\n", code[i], code_len[i]);
	}
	fclose(fw);
}
	
	
	
