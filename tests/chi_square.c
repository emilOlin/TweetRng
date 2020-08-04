/*
 * Program for testing randomness for files with the chisquared method
 *
 * outputs a V that can be checked against a table of chi-squared values to dertermine
 * if the tested file passes the test. If for the specific degrees of freedom the V is
 * smaller than the 1% or larger than the 99% of the table values the file can be asumed not
 * to be evenly distributed. The degree of freedom is one less than the amount of partitions.
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

const int numPartitions = 51;
FILE *inputFile;

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
	printf("expectedAmountOfNumbers: %f\n", expectedAmountOfNumbers);
	//Calculate V for every partition, last two partitions should be combined
	for (int i = 0; i < numPartitions - 1; i++){
		value = pow(partitions[i], 2)/expectedAmountOfNumbers;
		v += value;
		printf("%f\t%f\n", value, value/expectedAmountOfNumbers);
	}
	printf("\n");
	value = pow(partitions[numPartitions-1]+partitions[numPartitions], 2)/expectedAmountOfNumbers;
	v += value;
	v -= amountOfNumbers;
	printf("V %f, k %d, degrees of freedom %d\n", v, numPartitions, numPartitions - 1);

	return v;
}



int main(int argc, char const *argv[]) {
	if (argc < 2) {
		printf("not enough arguments\n");
		return 1;
	}
	//Do chi-squared for all outputing generators
	inputFile = fopen(argv[1],"rb");
	chiSquare();

	return 0;
}
