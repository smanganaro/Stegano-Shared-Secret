#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h> 
#include "random.h"

int64_t seed;

void setSeed(int64_t s){
	seed = (s ^ 0x5DEECE66DL) & ((1LL << 48) - 1);
}
uint8_t nextChar(void){
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
	return (uint8_t)(seed>>40);
} 