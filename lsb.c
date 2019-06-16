#include "lsb.h"

void lsb_2(uint8_t byte, uint8_t *pixels, size_t pos){

    int j;
    for (j=0; j<8; j+=2){
        int i = j/2;
        uint8_t to_save = (GET_BIT(byte, 8-1-j) << 1) | GET_BIT(byte, 8-1-(j+1));
        pixels[i] = (pixels[i] & ~(BIT(pos+1) | BIT(pos))) | (to_save << pos);
    }
}