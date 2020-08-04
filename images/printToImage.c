/*
 * Program to create a test image for graphical tests, currently works by
 * Taking four bytes at a time from the file temp.bin and putting a color
 * value of 255 255 255 at the point (prev number, current number) on the
 * grid
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
	FILE *imageFile;
	int x,y,pixel,height=1000,width=1000;

	//open image file to print to, can take one as an input
	if( 2 < argc){
	imageFile=fopen(argv[2],"wb");
	} else {
	imageFile=fopen("image.ppm","wb");
	}
	if(imageFile==NULL){
		perror("ERROR: Cannot open output file");
		exit(EXIT_FAILURE);
	}

	//formatting for ppm
	fprintf(imageFile,"P6\n");               // P6 filetype
	fprintf(imageFile,"%d %d\n",width,height);   // dimensions
	fprintf(imageFile,"255\n");              // Max pixel

	FILE *inputFile;
	inputFile = fopen(argv[1], "r");

	//Remove scrap data form matrix
	unsigned short imag[height][width];
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			imag[i][j] = 0;
		}
	}

	//Current and previous number
	unsigned int left[1];
	unsigned int right[1];

	//first read for prev number
	fread(left, 4, 1, inputFile);
	int maxPoints = height * width / 2;

	//read every 4 bytes from file as a int
	while(!feof(inputFile) && (maxPoints > 0)){
		fread(right, 4, 1, inputFile);

		//make compatible for matrix
		left[0] = left[0] % height;
		right[0] = right[0] % width;

		imag[left[0]][right[0]]++;

		left[0] = right[0];
		maxPoints--;
	}

	//Values to be printed
	unsigned char found[3];
	found[0] = 0xff;
	found[1] = 0xff;
	found[2] = 0xff;


	for(int i = 0; i < height; i++){
		for(int j = 0; j  <width; j++){
			//Change what is printed for every instance in the matrix
			for(int k = 0; k < imag[i][j]; k++){
				found[0] = 0x0;
				found[1] = 0x0;
				found[2] = 0x0;
			}
			fwrite(found, 1, 3, imageFile);
			found[0] = 0xff;
			found[1] = 0xff;
			found[2] = 0xff;
		}
	}
	fclose(imageFile);
}
