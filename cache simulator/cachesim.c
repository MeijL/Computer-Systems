#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "cachelab-tools.h"
#include <math.h>

typedef struct Node
{
	long long unsigned tag;
	int start;
	int isValid;
} Node;



int main( int argc, char *argv[])
{
FILE *traceFile;
char instr;
long long unsigned address;
int size;

int setBits;
int blockBits;
int lines;

int sets;

long long unsigned tag;
int setLoc;

int bitOps;

char opt;
char* fileName;

int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

int hitSentinel;

int cacheCount = 1;

//traceFile = fopen("traces/yi2.trace", "r");


setBits = 0;
blockBits = 1;

while( (opt = getopt(argc, argv, "b:s:E:t:")) != -1)
{
	switch(opt){
	case 'b':
		blockBits = atoi(optarg);
		break;
	case 's':
		setBits = atoi(optarg);
		break;
	case 'E':
		lines = atoi(optarg);
		break;
	case 't':
		fileName = optarg;
		traceFile = fopen(optarg, "r");
		break;	}
}


sets = pow(2, setBits);
Node cache[sets][lines];


for(int i = 0; i < sets; i++ )
{
	for(int j = 0; j < lines; j++)
	{
	
		cache[i][j].start = 0;
	}
}

//???
fscanf(traceFile, "%c %llx, %d\n", &instr, &address, &size);
while(fscanf(traceFile, "%c %llx, %d\n", &instr, &address, &size) != EOF)
{
	if(instr == 'I')
	{
		continue;
	}
	if(instr == 'M')
	{
		hit_count++;
	}
	cacheCount++;

	address = address >> blockBits;
	
	bitOps = ~((-1) << setBits);
	setLoc = bitOps & address;
	
	
	address = (address >> setBits) & (~((1<<31) >> (blockBits + setBits -1 ))) ;
	
	tag = address;	
	

	int savedNode;
	hitSentinel = 0;
	for(int j = 0; j < lines; j++)
	{
	 	if(cache[setLoc][j].start != 0)
		{
			if(cache[setLoc][j].tag == tag)
			{
				hit_count++;
				hitSentinel = 1;
				(cache[setLoc][j].start) = cacheCount;
				savedNode = j;
				break;
			}
		}
			//printf("good");
	}
	if(hitSentinel != 1)
	{
		if(cache[setLoc][savedNode].start != 0)
		{
			eviction_count++;
		}
		miss_count++;
		savedNode = 0;
		for(int j = 0; j < lines; j++)
		{
			if(cache[setLoc][savedNode].start > cache[setLoc][j].start)
			{
				savedNode = j;
			}
		}
		cache[setLoc][savedNode].tag = tag;
		
		cache[setLoc][savedNode].start = cacheCount;
		
	}
	/*for(int i = 0; i < sets; i++ )
	{
	curNode = cache[i];
	while(curNode != NULL)
        {
		lastNode = curNode->next;
		free(curNode);
		curNode = lastNode;
        }
	}*/
	
	//printf("%c %llx %d\n",instr, address, size);
}
fclose(traceFile);
printCachesimResults(hit_count, miss_count, eviction_count);
}
