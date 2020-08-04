/*
 * This program takes in the name of one file and runs the tests on it.
 */
#include "gdef.h"
#include "swrite.h"
#include "unif01.h"
#include "ugfsr.h"
#include "ulcg.h"
#include "bbattery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

FILE *inputFile;
const int numPartitions = 51;

double chiSquare(){
	double v = 0;
	unsigned int amountOfNumbers = 0;
	double expectedAmountOfNumbers = 0;
	unsigned int partitions[numPartitions+1];
	unsigned int input[1] = {0};

	for (int i = 0; i < numPartitions+1; i++){
		partitions[i] = 0;
	}

	//Read the file with numbers and add to corresonding partition
	while(!feof(inputFile)){
		fread(input, 4, 1, inputFile);
		amountOfNumbers++;
		partitions[(unsigned int)(((long double)input[0]/UINT_MAX)*numPartitions)]++;
	}

	printf("amountOfNumbers: %d\n", amountOfNumbers);

	expectedAmountOfNumbers = (double) amountOfNumbers/numPartitions;
	double value = 0;

	//Calculate V for every partition
	for (int i = 0; i < numPartitions - 1; i++){
		value = pow(partitions[i], 2)/expectedAmountOfNumbers;
		v += value;
	}
	value = pow(partitions[numPartitions-1]+partitions[numPartitions], 2)/expectedAmountOfNumbers;
	v += value;
	v -= amountOfNumbers;
	printf("V: %f, k: %d, degrees of freedom: %d\n", v, numPartitions, numPartitions - 1);

	return v;
}


unsigned int fileRead(void){
	unsigned int ret[1];
	fread(ret, sizeof(int), 1, inputFile);
	if(feof(inputFile)){
		fseek(inputFile,0, SEEK_SET);
	}
	return *ret;
}
int main (int argc, char const *argv[]){

	unif01_Gen *gen;

	swrite_Basic = FALSE;

	if(argc == 2){
		inputFile = fopen(argv[1],"rb");
	} else {
		printf("No input file for testsing\n");
		return 0;
	}

	//create generator
	gen = unif01_CreateExternGenBits("fileRead", fileRead);

	//first testing battery
	bbattery_SmallCrush(gen);

	//reset file pos
	fseek(inputFile, 0, SEEK_SET);

	//second testing battery
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	fclose(inputFile);
	return 0;
}
