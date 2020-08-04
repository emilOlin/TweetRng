CC = gcc

FLAGS = -ltestu01 -lprobdist -lmylib -lm

FOO?=

LCMAddUtf: generators/tweetRngLCMAddUtf.c
	$(CC) generators/tweetRngLCMAddUtf.c -o generators/tweetRngLCMAddUtf.out $(FLAGS)
	generators/tweetRngLCMAddUtf.out
	$(CC) -o tests/singleTest.out tests/singleTest.c $(FLAGS)
	tests/singleTest.out binFiles/tweetRngLCMAddUtf.bin
	$(CC) images/printToImage.c -o images/printToImage.out
	images/printToImage.out binFiles/tweetRngLCMAddUtf.bin images/tweetRngLCMAddUtf.ppm
	convert images/tweetRngLCMAddUtf.ppm -scale 400% images/tweetRngLCMAddUtf.png

allTests: tests/testAll.c
	$(CC) tests/testAll.c -o tests/allTest.out $(FLAGS)
	tests/allTest.out > tests/testResult

# to run write, make chiTest FOO=<location of bin file>
chiTest: tests/testAll.c
	$(CC) tests/chi_square.c -o tests/chi_square.out $(FLAGS)
	tests/chi_square.out $(FOO)

createBin: generators/tweetRngLessThanByte.c generators/tweetRngBitPerByte.c generators/tweetRngMulAddUtf.c generators/tweetRngBitPerUtf.c generators/MTprinter.c generators/tweetRngLCMAddUtf.c generators/LCMprinter.c generators/LCMprinterMinimumStandard.c generators/tweetRngLessThanUtf.c
	$(CC) generators/tweetRngLessThanByte.c		-o generators/tweetRngLessThanByte.out
	$(CC) generators/tweetRngLessThanUtf.c 		-o generators/tweetRngLessThanUtf.out
	$(CC) generators/tweetRngBitPerByte.c 		-o generators/tweetRngBitPerByte.out
	$(CC) generators/tweetRngBitPerUtf.c 		-o generators/tweetRngBitPerUtf.out
	$(CC) generators/tweetRngMulAddUtf.c 		-o generators/tweetRngMulAddUtf.out
	$(CC) generators/tweetRngLCMAddUtf.c 		-o generators/tweetRngLCMAddUtf.out
	$(CC) generators/MTprinter.c 			-o generators/MTprinter.out $(FLAGS)
	$(CC) generators/LCMprinter.c 			-o generators/LCMprinter.out $(FLAGS)
	$(CC) generators/LCMprinterMinimumStandard.c 	-o generators/LCMprinterMinimumStandard.out $(FLAGS)
	generators/tweetRngLessThanByte.out
	generators/tweetRngLessThanUtf.out
	generators/tweetRngBitPerByte.out
	generators/tweetRngBitPerUtf.out
	generators/tweetRngMulAddUtf.out
	generators/tweetRngLCMAddUtf.out
	generators/MTprinter.out
	generators/LCMprinter.out
	generators/LCMprinterMinimumStandard.out

createImages: images/printToImage.c
	$(CC) images/printToImage.c -o images/printToImage.out
	images/printToImage.out binFiles/tweetRngLessThanByte.bin 		images/tweetRngLessThanByte.ppm
	convert images/tweetRngLessThanByte.ppm 		-scale 400% images/tweetRngLessThanByte.png
	images/printToImage.out binFiles/tweetRngLessThanUtf.bin 		images/tweetRngLessThanUtf.ppm
	convert images/tweetRngLessThanUtf.ppm 			-scale 400% images/tweetRngLessThanUtf.png
	images/printToImage.out binFiles/tweetRngMulAddUtf.bin 			images/tweetRngMulAddUtf.ppm
	convert images/tweetRngMulAddUtf.ppm 			-scale 400% images/tweetRngMulAddUtf.png
	images/printToImage.out binFiles/tweetRngBitPerByte.bin 		images/tweetRngBitPerByte.ppm
	convert images/tweetRngBitPerByte.ppm 			-scale 400% images/tweetRngBitPerByte.png
	images/printToImage.out binFiles/tweetRngBitPerUtf.bin 			images/tweetRngBitPerUtf.ppm
	convert images/tweetRngBitPerUtf.ppm 			-scale 400% images/tweetRngBitPerUtf.png
	images/printToImage.out binFiles/tweetRngLCMAddUtf.bin 			images/tweetRngLCMAddUtf.ppm
	convert images/tweetRngLCMAddUtf.ppm 			-scale 400% images/tweetRngLCMAddUtf.png
	images/printToImage.out binFiles/MTrandomness.bin 			images/MTrandomness.ppm
	convert images/MTrandomness.ppm 			-scale 400% images/MTrandomness.png
	images/printToImage.out binFiles/LCMrandomness.bin 			images/LCMrandomness.ppm
	convert images/LCMrandomness.ppm 			-scale 400% images/LCMrandomness.png
	images/printToImage.out binFiles/LCMrandomnessMinimumStandard.bin 	images/LCMrandomnessMinimumStandard.ppm
	convert images/LCMrandomnessMinimumStandard.ppm 	-scale 400% images/LCMrandomnessMinimumStandard.png

analyseData: data_analysis/analyser.py
	python data_analysis/analyser.py > data_analysis/analysis

# Downloads about 60 MB of data
createManyTweets: tweetDownloaders/getManyTweets.py
	python tweetDownloaders/getManyTweets.py >> manytweets

clean:
	rm -r *.o *.out *.bin *.ppm */*.o */*.out */*.bin */*.ppm

median: data_analysis/median.py
	python data_analysis/median.py
