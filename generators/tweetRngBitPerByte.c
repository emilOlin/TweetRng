/*
 * RNG that takes the least significant bit from every byte in the data
 * This is done for every bit in the 32 bit number
 *
 */
#include <stdio.h>
#include <stdlib.h>

unsigned int randNum = 1;
void tweetRng(FILE *tweets){

	int numberOfChars = 32;
	unsigned char cur;

	// load in the bytes and take the least significant bit
	while(!feof(tweets) && (numberOfChars > 0)){
		cur = fgetc(tweets);
		cur = cur & 1;
		randNum = (randNum << 1) + cur;

		numberOfChars--;
	}

	// if we run out of characters in the data file, go to the top of the file
	if(feof(tweets)){
		fseek(tweets, 0, SEEK_SET);
	}
}

int main(){
	FILE *out;
	FILE *tweets;
	tweets = fopen("manytweets", "r");
	out = fopen("binFiles/tweetRngBitPerByte.bin", "w");

	//create the numbers and save them to a file
	unsigned int num[1];
	for(int i = 0; i < 52000000; ++i){
		tweetRng(tweets);
		num[0] = randNum;
		fwrite( num, 1, 4, out);
	}
	fclose(tweets);
	fclose(out);
	return 0;
}
