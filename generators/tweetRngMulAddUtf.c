/*
 * Inspired by the Linear Congruential Method
 * RNG that takes in an utf-8 character and multiplies the value of it with the current random number.
 * It then adds the value of the least significant byte to the random number as a constant.
 * utf-8 characters vary in the amount of bytes they use, whole bytes between one and four
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>

// position in the file that contains the data

unsigned int randNum = 1;

void tweetRng(FILE *tweets){

	int numberOfChars = 32;
	unsigned int firstByte;
	unsigned int secondByte;
	unsigned int thirdByte;
	unsigned int fourthByte;

	// utf-8 has varying byte length, get whole character and do the multiplication and addition
	while(!feof(tweets) && (numberOfChars > 0)){

		firstByte = fgetc(tweets);
		if(firstByte & 0b10000000){
			secondByte = fgetc(tweets);

			if(firstByte & 0b00100000){
				thirdByte = fgetc(tweets);

				if(firstByte & 0b00010000){
					fourthByte = fgetc(tweets);
					randNum = (randNum) * ((firstByte << 24) + (secondByte << 16) + (thirdByte << 8) + fourthByte) + fourthByte;
				} else {
					randNum = (randNum) * ((firstByte << 16) + (secondByte << 8) + thirdByte) + thirdByte;
				}
			} else {
				randNum = (randNum) * ((firstByte << 8) + secondByte) + secondByte;
			}
		} else{
			randNum = (randNum) * (firstByte) + firstByte;
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
	out = fopen("binFiles/tweetRngMulAddUtf.bin", "w");
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
