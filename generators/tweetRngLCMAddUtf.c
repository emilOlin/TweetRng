/*
 * RNG inspired by the linear congruential method, this RNG gives the best result
 * Multiplies the previous number by 11 and adds the five least significant bits of a character
 * This is done multiple times every time a number is generated
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int randNum = 1;

//gets a character from the tweet data, can be between 1 and 4 bytes long
unsigned int getChar(FILE *tweets){
	unsigned int startOfChar;
	unsigned int secondOfChar;
	unsigned int thirdOfChar;
	unsigned int fourthOfChar;
	unsigned int ret;

	// load bytes until entire character has been extracted
	startOfChar = fgetc(tweets);
	if(startOfChar & 0b10000000){
		secondOfChar = fgetc(tweets);
		if(startOfChar & 0b00100000){
			thirdOfChar = fgetc(tweets);
			if(startOfChar & 0b00010000){
				fourthOfChar = fgetc(tweets);
				ret = (startOfChar << 24) + (secondOfChar << 16) + (thirdOfChar << 8) + fourthOfChar;
			} else {
				ret = (startOfChar << 16) + (secondOfChar << 8) + thirdOfChar;
			}
		} else {
			ret = (startOfChar << 8) + secondOfChar;
		}
	} else if(startOfChar){
		ret = startOfChar;
	}
	if(feof(tweets)){
		//printf("roundhouse\n");
		fseek(tweets, 0, SEEK_SET);
	}
	return ret;
}

// the RNG, generates a set amount of random numbers or until out of data to work with
void tweetRng( FILE *tweets){
	int numberOfChars = 13;

	unsigned int cur;
	while(numberOfChars > 0){
		cur = getChar(tweets);
		//randNum = (randNum * 48271) + (cur & 0x1f);
		randNum = (randNum * 11) + (cur & 0x1f);
		numberOfChars--;
	}
}

int main(){
	FILE *out;
	FILE *tweets;
	tweets = fopen("manytweets", "r");

	out = fopen("binFiles/tweetRngLCMAddUtf.bin", "w");

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
