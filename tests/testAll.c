/*
* Run testing batteries for all generators that output to bin files as well as
* minimum standard and one found in the documentation for testU01
*
* The binfiles must exist before running the tests
*/

#include "gdef.h"
#include "swrite.h"
#include "unif01.h"
#include "ugfsr.h"
#include "ulcg.h"
#include "bbattery.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *inputFile;

const int numPartitions = 100;

// chi quared test taken from the file with the same name
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
int main (void){
	unif01_Gen *gen;

	swrite_Basic = FALSE;


	inputFile = fopen("binFiles/tweetRngLessThanByte.bin","rb");
	gen = unif01_CreateExternGenBits("tweetRng Less Than Byte", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/tweetRngLessThanUtf.bin","rb");
	gen = unif01_CreateExternGenBits("tweetRng Less Than Utf", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/tweetRngMulAddUtf.bin","rb");
	gen = unif01_CreateExternGenBits("tweetRng Mul Add Utf", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/tweetRngBitPerUtf.bin","rb");
	gen = unif01_CreateExternGenBits("tweetRng Bit Per Utf", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/tweetRngBitPerByte.bin","rb");
	gen = unif01_CreateExternGenBits("tweetRng Bit Per Byte", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/tweetRngLCMAddUtf.bin","rb");
	gen = unif01_CreateExternGenBits("tweetRng LCM Add Utf", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/MTrandomness.bin","rb");
	gen = unif01_CreateExternGenBits("MT randomness from file", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/LCMrandomness.bin","rb");
	gen = unif01_CreateExternGenBits("LCM randomness", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");

	inputFile = fopen("binFiles/LCMrandomnessMinimumStandard.bin","rb");
	gen = unif01_CreateExternGenBits("LCM randomness Minimum Standard", fileRead);
	bbattery_SmallCrush(gen);
	fseek(inputFile, 0, SEEK_SET);
	bbattery_Rabbit(gen, 1048576);
	unif01_DeleteExternGenBits(gen);
	printf("========= Summary results of chi squared =========\n\n");
	fseek(inputFile, 0, SEEK_SET);
	chiSquare();
	printf("\n\n");
	fclose(inputFile);
	printf("----------------------------------------------------------------------\n\n");
	return 0;
}
