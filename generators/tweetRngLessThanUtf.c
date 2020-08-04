/*
 * RNG that does the same as tweetRngByteLessThan but with all bytes instead
 * Checks if the bits that contain the character belongs to the upper half or the lower half of the possible values
 * for the different byte sizes.
 * If it is the smaller half a one is put in the least significant bit, otherwise zero
 * This is repeated for all 32 bits in the number
 */
#include <stdio.h>
#include <stdlib.h>

unsigned int randNum = 0;

void tweetRng(FILE *tweets){
	int numberOfChars = 32;
	unsigned int startOfChar;
	unsigned int secondOfChar;
	unsigned int thirdOfChar;
	unsigned int fourthOfChar;

	//Get the character and do operations based on the byte size
	while(!feof(tweets) && (numberOfChars > 0)){
		startOfChar = fgetc(tweets);
		if(startOfChar & 0b10000000){
			secondOfChar = fgetc(tweets);
			if(startOfChar & 0b00100000){
				thirdOfChar = fgetc(tweets);
				if(startOfChar & 0b00010000){
					fourthOfChar = fgetc(tweets);
					randNum = (randNum << 1) + (((startOfChar << 24) + (secondOfChar << 16) + (thirdOfChar << 8) + fourthOfChar) < 4036990083);
				} else {
					randNum = (randNum << 1) + (((startOfChar << 16) + (secondOfChar << 8) + thirdOfChar) < 14909849);
					numberOfChars--;
				}
			} else {
				randNum = (randNum << 1) + (((startOfChar << 8) + secondOfChar ) < 55469);
				numberOfChars--;
			}
		} else if(startOfChar){
			randNum = (randNum << 1) + (startOfChar < 101);
			numberOfChars--;
		}
	}

	//check if the end of the file has been reached and reset position in the file
	if(feof(tweets)){
		fseek(tweets, 0, SEEK_SET);
	}
}

int main(){
	FILE *tweets;
	tweets = fopen("manytweets", "r");

	FILE *out;
	out = fopen("binFiles/tweetRngLessThanUtf.bin", "w");

	int str[1];
	for(int i = 0; i < 52000000; ++i){
		tweetRng(tweets);
		str[0] = randNum;
		fwrite( str, 1, 4, out);
	}
	fclose(tweets);
	fclose(out);
	return 0;
}
