/*
 * RNG that takes the least signigicant bits from 32 utf-8 characters and combines them into a random number
 *
 */
#include <stdio.h>
#include <stdlib.h>

unsigned int randNum = 0;

unsigned int tweetRng(FILE *tweets){
	int numberOfChars = 32;
	unsigned int firstByte;
	unsigned int secondByte;
	unsigned int thirdByte;
	unsigned int fourthByte;

	// utf-8 has varying byte length, get whole character and do operations
	while(!feof(tweets) && (numberOfChars > 0)){
		firstByte = fgetc(tweets);
		if(firstByte & 0b10000000){
			secondByte = fgetc(tweets);

			if(firstByte & 0b00100000){
				thirdByte = fgetc(tweets);

				if(firstByte & 0b00010000){
					fourthByte = fgetc(tweets);
					randNum = (randNum << 1) + (fourthByte & 0x1);

				} else {
					randNum = (randNum << 1) + (thirdByte & 0x1);
				}
			} else {
				randNum = (randNum << 1) + (secondByte & 0x1);
			}
		} else {
			randNum = (randNum << 1) + (firstByte & 0x1);
		}

		numberOfChars--;
	}

	// if we run out of characters in the data file, go to the top of the file
	if(feof(tweets)){
		fseek(tweets, 0, SEEK_SET);
	}
	return randNum;
}


int main(){
	FILE *out;
	out = fopen("binFiles/tweetRngBitPerUtf.bin", "w");
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
