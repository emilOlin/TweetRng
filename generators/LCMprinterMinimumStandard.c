/*
 * RNG taken from testU01 using the linear congruential method
 * This is used to compare the randomness of our random number generators
 */

#include "gdef.h"
#include "swrite.h"
#include "unif01.h"
#include "ulcg.h"
#include "bbattery.h"
#include <stdio.h>
#include <stdlib.h>

int main (void){
	unif01_Gen *gen;

	FILE *fp;
	fp = fopen("binFiles/LCMrandomnessMinimumStandard.bin", "w");

	gen = ulcg_CreateLCG(2147483647, 48271, 0, 1);
	int str[1];
	for(int i = 0; i < 51320000; i++){
		str[0] = gen->GetBits(gen->param, gen->state);
		fwrite(str, 1, 4, fp);
	}
	ulcg_DeleteGen(gen);

	return 0;
}
