/*
 * RNG that does the same as greaterForUtf but with the four least significant bits of every byte
 * If the value of the four bits is smaller than half of the max value a one is put in the least significant bit
 * This is done for all the 32 bits in the number
 *
 */
#include <stdio.h>
#include <stdlib.h>

unsigned int randNum = 1;

void tweetRng(FILE *tweets){

	int numberOfChars = 32;
	unsigned char cur;

	// takes the four least significant bits of a byte
	// if small next bit is 0, if large it is 1
	while(!feof(tweets) && (numberOfChars > 0)){
		cur = fgetc(tweets);
		cur = cur & 0xf;
		if(cur < 8){
			randNum = randNum << 1;
		} else {
			randNum = (randNum << 1) + 1;
		}
		numberOfChars--;
	}

	// if we run out of characters in the data file, go to the top of the file
	if(feof(tweets)){
		fseek(tweets, 0, SEEK_SET);
	}
}

int main(){
	FILE *out;
	out = fopen("binFiles/tweetRngLessThanByte.bin", "w");
	FILE *tweets;
	tweets = fopen("manytweets", "r");


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
