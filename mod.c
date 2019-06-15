#include <stdio.h>
#include <stlib.h>

#define MOD 251

uint32_t mod(uint32_t n){
	return n > MOD ? n % MOD : n;
}

uint32_t mod_sum(uint32_t a, uint32_t b){
	return mod(a+b);
}

uint32_t mod_sub(uint32_t a, uint32_t b){
  uint32_t n1 = mod(a);
  uint32_t n2 = mod(b);
  uint32_t ans = 0;
  if (n1 < n2){
  	ans = n1 + MOD - n2;
  }
  return ans;
}

uint32_t mod_mul(uint32_t a, uint32_t b){
	long double x;
  	uint32_t c;
  	int32_t r;
  	if (a >= MOD)
    	a %= MOD;
  	if (b >= MOD)
    	b %= MOD;
  	x = a;
  	c = (x * b) / MOD;
  	r = (int32_t)(a * b - c * MOD) % (int32_t)MOD;
  	return r < 0 ? r + MOD : r;
}

uint32_t mod_inv(uint32_t a){
	uint32_t ans;
	a %= MOD;
	ans = MOD - a;
	return ans;
}

uint32_t mod_div(uint32_t a, uint32_t b){
	return mod_mul(a, mod_inv(b));
}