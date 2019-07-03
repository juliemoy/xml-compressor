#include <stdio.h>
#include <stdlib.h>

#define SIZE 52
#define N 26

int M, L;
char string[] = "IS THIS STILL WORKING";
int count[SIZE] = {0}, heap[SIZE]={0}, dad[SIZE]={0};
int len[N], code[N];
int num_bits = 0;

//function list
void print();
void get_count();
void get_heap();
void downheap(int k);
void construct();
void encode();
int code_index(char c);
unsigned bits(unsigned s, int k, int j);
void code_string();

void main()
{
	get_count();
	get_heap();
	construct();
	print();
	encode();
	code_string();
}

void print()
{
	int i; 
	printf("M: %d\n", M);
	printf("    k: ");
	for(i=0; i<N+L; i++)printf("%3d", i);
	printf("\n");
	printf("count: ");
	for(i=0; i<N+L; i++) printf("%3d", count[i]);
	printf("\n");
	printf("  dad: ");
	for(i=0; i<N+L; i++) printf("%3d", dad[i]);
	printf("\n");
}
		
void get_count()
{
	int i=0,j=1;
	for(; j<=SIZE;j++) count[j] = 0;
	
	while(string[i] !='\0')
	{
		if(string[i] == ' ') count[0]++;
		else count[(string[i]-'A')+1]++;
		i++;
	}
}

void get_heap()
{
	int i,k;
	for(i=0, M=0; i<N; i++)
		if(count[i])heap[++M] = i;
	L = M;
	for(k = M/2; k > 0; k--) downheap(k);
}

void downheap(int k)
{
	int j, v, h;
	h = heap[k];
	v = count[heap[k]];
	while (k<=M/2)
	{
		j = (2*k);
		if(j<M && count[heap[j]] > count[heap[j+1]]) j++;
		if(v <= count[heap[j]]) break;

		heap[k] = heap[j];
		k = j;
	}
	heap[k] = h;
}

void construct()
{
	int t;
	while(M > 1)
	{
		t = heap[1]; heap[1] = heap[M--];
		printf("downheaping %d\n", count[heap[1]]);
		downheap(1);
		count[N+M] = count[heap[1]] +count[t];
		printf("count[%d] = %d\n", N+M, count[N+M]);
		dad[t] = N+M; dad[heap[1]] = -N-M;
		heap[1] = N+M; downheap(1);
	}
	dad[N+M] = 0;
}

void encode()
{
	int k,i,j,t,x;
	for(k=0; k <= N; k++)
		if(!count[k]){code[k]=0; len[k]=0;}
		else
		{
			i=0; j=1; t=dad[k]; x=0;
			while(t)
			{
				if(t<0){x+=j; t = -t;}
				t=dad[t]; j+=j; i++;
			}
			code[k] = x; len[k] = i;
		}
		
	printf("code: ");
	for(i=0; i<=N; i++) printf("%3d", code[i]);
	printf("\n len: ");
	for(i=0; i<=N; i++) printf("%3d", len[i]);
	printf("\n");
} 

int code_index(char c)
{
	if(c==' ')return 0;
	else return c-'A'+1;
}

unsigned bits(unsigned x, int k, int j)
{ return (x >> k) & ~(~0<<j);}

void code_string()
{
	int j=0,i; 
	FILE* fp = NULL;
	
	fp = fopen("bits.txt", "w");
	while(string[j] != '\0')
	{
		for(i=len[code_index(string[j])]; i>0; i--){
			fprintf(fp,"%1d", bits(code[code_index(string[j])], i-1, 1));
			num_bits++;
		}
		//fprintf(fp,"%c", string[j]);
		j++;
	}
	fclose(fp);
	
	fp = fopen("code.txt", "w");
	for(i=0; i<=N; i++)fprintf(fp,"%3d", code[i]);
	fprintf(fp, "\n");
	for(i=0; i<=N; i++)fprintf(fp,"%3d", len[i]);
	fprintf(fp, "\n");
	fprintf(fp, "%d", num_bits);
	fclose(fp);
}
			
