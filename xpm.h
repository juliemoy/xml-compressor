#ifndef XPM_H
#define XPM_H

#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000
#define SYMBOL_LENGTH 4
#define TRUE 1
#define FALSE 0
//#define filename "landscape.xpm"

//files used in program
static const char* filename = "small_fish.xpm";
static const char* bitfile = "bits.bin";
static const char* codefile = "code.txt";
static const char* newfile = "decompressed.xpm";

//variables used
FILE* fp;
int width, length, colors, symbol;
int count[SIZE], heap[SIZE], dad[SIZE];
int code[SIZE], code_len[SIZE];
char* line;
char symbols[SIZE][SYMBOL_LENGTH];
size_t len;
int heap_size, const_heap;

//node used in decompress for binary search tree
typedef struct node
{
	struct node* l;
	struct node* r;
	int key;		//index of symbol in array
}node;

//functions
//xpm_main
int main();
void initialize();
//compress.c
void get_info();
void get_symbols();
void get_count();
int symbol_search(char* str);
void get_heap();
void downheap(int k);
void construct();
void encode();
int bits(int s, int k, int j);
void write();
void write_code();

//decompress.c
void get_code_info();
void get_arrays();
void make_tree();
void decode();
void deallocate(node* x);

#endif
