#include <stdio.h>
#include <stdlib.h>
#include "stego.h"

#define MAX 50

char ** generateRandomMatrix(int a, int b){
	
	//TODO

	int i;
 	uint8_t num;
 	setSeed(SET);
	for (i = 0; i < MAX;i++)
	{
 		num = nextChar();
 		printf("%d\t", num);
 	}
 	
 	return NULL ;
}
