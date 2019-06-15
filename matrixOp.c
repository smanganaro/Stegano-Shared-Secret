#include <stdio.h>
#include <stdlib.h>
#include "stego.h"

#define MAX 50





char matrix_t* random_matrix(uint32_t rows, uint32_t cols){
	
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
