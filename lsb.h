#ifndef LSB_H
#define LSB_H

#include <stdio.h>
#include <stdint.h>

#define BIT(n) (0x01 << (n))
#define GET_BIT(x, n) ((x >> n) & 1)

void lsb_2(uint8_t byte, uint8_t *pixels, size_t pos);


#endif LSB_H
