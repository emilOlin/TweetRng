/*
 * RNG taken from testU01 using the Mersenne Twister
 * This is used to compare the randomness of our random number generators
 */
 
#include "gdef.h"
#include "swrite.h"
#include "unif01.h"
#include "ugfsr.h"
#include "bbattery.h"
#include <stdio.h>
#include <stdlib.h>

int main (void){
	unif01_Gen *gen;

	FILE *fp;
	fp = fopen("binFiles/MTrandomness.bin", "w");

	swrite_Basic = FALSE;
	gen = ugfsr_CreateMT19937_98(234231);
	int str[2];
	for(int i = 0; i < 51320000; i++){
		str[0] = gen->GetBits(gen->param, gen->state);
		fwrite(str, 1, 4, fp);
	}
	ugfsr_DeleteGen(gen);

	return 0;
}
