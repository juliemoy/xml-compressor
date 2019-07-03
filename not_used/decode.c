#include <stdio.h>
#include <stdlib.h>

#define SIZE 50

int N = 26;
int code[SIZE];
int len[SIZE];
FILE* fp = NULL;
int num_bits=0;

typedef struct node
{
	struct node* l;
	struct node* r;
	char key;
}node;

node* z, *head;

//function list
int get_int();
int bits(int s, int k, int j);
char get_key(int index);
void make_tree();
void get_arrays();
void decode();
void deallocate(node* root);

int main()
{
	get_arrays();
	make_tree();
	printf("decoded message: \n");
	decode();
	deallocate(head);
	printf("\n");
}

int get_int()
{
	char a, b, c;
	int one, two, three;
	a = fgetc(fp);
	b = fgetc(fp);
	c = fgetc(fp);
	
	if(a != ' ') one = (a-'0')*100; else one = 0;
	if(b != ' ') two = (b-'0')*10; else two = 0;
	if(c != ' ') three = (c-'0'); else three = 0;
	
	return one+two+three;
}

int bits(int x, int k, int j)
{ return (x >> k) & ~(~0<<j);}

char get_key(int index)
{
	if(index == 0) return ' ';
	else return index + 'A' -1;
}

void make_tree()
{
	int i, b, bit;
	z = (node*)malloc(sizeof(node*));
	head = (node*)malloc(sizeof(node*));
	head->l = z;
	head->r = z;
	head->key = 0;
	
	node* p, *x;
	
	for(i=0; i<=N; i++)
	{
		x = head;
		b = len[i];
		while(b > 0)
		{
			p=x; 
			if ((bit = bits(code[i], --b, 1)) != 0)
			{
				x=x->r;
				//printf("moved right\n");
			}
			else{
				x = x->l;
				//printf("moved left\n");
			}
			if(x==z)
			{
				x = (node*)malloc(sizeof(node*));
				x->l = z; x->r = z; x->key = 'a';
				if(bit != 0) p->r=x; else p->l=x;
			}
			
		}
		x->key = get_key(i); //x->l=z; x->r=z;
		//if(bits(code[i], b+1, 1)) p->r=x; else p->l=x;
		//printf("insterted %c, %d\n", x->key, code[i]);
	}
}

void get_arrays()
{
	int i;
	fp = fopen("code.txt", "r");
	//printf("opened file\n");
	//get code array
	for(i=0; i<=N; i++) code[i] = get_int();
	//for(i=0; i<=N; i++) printf("%3d", code[i]);
	fgetc(fp);
	//printf("\n");
	
	//get len array
	for(i=0; i<=N; i++) len[i] = get_int();
	//for(i=0; i<=N; i++) printf("%3d", len[i]);printf("\n");
	
	//get number of bits
	fscanf(fp, "%d", &num_bits);
	fclose(fp);
	//printf("closed file\n");
	//printf("num bits: %d\n", num_bits);
}

void decode()
{
	fp = fopen("bits.txt", "r");
	//printf("file opened\n");
	int i = 0, bit;
	node* x = head;
	//printf("x created\n");
	
	fscanf(fp, "%1d", &bit);
	while(i<=num_bits)
	{
		if(bit){ 
			x = x->r;
			//printf("moved right\n");
			 }
		else {
			x=x->l;
			//printf("moved left\n");
			}
		//printf("key: '%c'\n", x->key);
		if(x->key != 'a'){printf("%c", x->key); x=head;}
		fscanf(fp, "%1d", &bit);
		i++;
	}	
}

void deallocate(node* x)
{
	if(x == z) return;
	
	deallocate(x->l);
	deallocate(x->r);
	//printf("deallocating %c\n", x->key);
	free(x);
}
	
	





