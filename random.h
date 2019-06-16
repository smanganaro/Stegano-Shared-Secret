#ifndef RANDOM_H
#define RANDOM_H

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

int64_t seed;
void setSeed(int64_t seed);
uint8_t nextChar(void);

#endif 