# TweetRNG
Random number generators based on tweets.

## Downloading the tweets
To download tweets from twitters database a developer account is needed to authenticate the connection between the program and the database with the aid of special keys. Creating an account and receiving keys can be done here: https://developer.twitter.com/en/apply-for-access

Tweets can be downloaded with ``` python getSetAmountOftweets.py X``` to get the text of X tweets
or by ```make createManyTweets``` to get 500 000 tweets, putting them in the manytweets file. For the programs to work the aforementioned keys need to be inserted into the top of the files where there are two empty strings.

## Generators
The different generators can all be run with the command `make createBin` to create the binary files of random numbers that are used in the testing. They do this by reading the tweets written down in `manytweets`. They create the numbers in this way as connection to the internet for every new random number takes too long time when generating a large amount of them. Furthermore twitter only allows a user to connect to their database a certain amount of times before you are locked out for a while. Before the binfiles can be created `manytweets` needs to have at least 50MB in it to ensure the generators do not fall into a predictable cycle due to the low number of tweets, the tests were run at 250MB.

#### tweetRngLCMAddUtf
The best version of tweetRNG, it uses 13 utf-8 characters per number and is generated with the formula:
```
randNum = (randNum * 11) + (cur & 0x1f);
```
The five least significant bits are those that changes the most in the twitter data stream and making use of more risks creating patterns in the numbers. This version was inspired by the linear congruential method but replaces the constants with values from the data stream. Using less characters than this causes some tests to fail.

All tests and image creation can be done specifically for this generator by using ´´´make LCMAddUtf´´´

#### tweetRngLessThanUtf
Creates a random number by checking if an utf-8 character is greater than or less than the median. This is done separately for each character size due to the uneven distribution of the number of bytes in the characters.
```
randNum = (randNum << 1) + (utf-8 char less than half)
```

#### tweetRngLessThanByte
Does the same as greater for utf but for every byte in the input heedless of the character sizes.

#### tweetRngMulAddUtf
This is also a version inspired by the linear congruential method, but this one both multiplies the random number with the utf-8 character and adds the least significant byte to it whereas the best version of tweetRNG has a static multiplier.

#### tweetRngBitPerUtf
Builds a number by putting together the least significant bit of the next 32 utf-8 characters.

#### tweetRngBitPerByte
Builds a number by putting together the least significant bit of the next 32 bytes, ignoring character length.

#### MTprinter
The testU01 version of the Mersenne twister random number generator.

#### LCMprinter
TestU01 version of a linear congruential method random number generator, utilizing the same constants as bestVersionOftweetRng.

#### LCMprinterMinimumStandard
TestU01 version of a linear congruential method random number generator, utilizing the constants recommended by the Minimum Standard.

## tests
### chi_square
The chi-square test can be run with ```make chiTest``` to test how uniform the number distribution in the binfiles are for all of the generators.

### singleTest
The singleTest takes in the location of one binfile to test that one specifically with the testU01 test batteries rabbit and small crush.

### testAll
Tests all of the bin files with the testing batteries rabbit, and small crush, and our own chi squared. Run with ```make allTests```.

## Analyser
The analyser.py file is used to analyse the data stream of tweets to find the amount of character chains, the most common characters, which bits change the most, what bits most often change together and the distribution of the different potential byte lengths the utf-8 characters have. Run with ```make analyseData```

## Tweets per second
The program streamtest.py connects to the twitter database every 30 minutes and counts tweets for 30 seconds to get the average tweets per second which is then stored in the file tdata.txt together with a timestamp. The twitter api states that the stream outputs roughly one percent of all tweets currently being posted and the average is therefore multiplied by 100. The program was run for one week but the data acquired from this program is by no means precise and should only be used to give a general idea of the posting activity on twitter.

The folder OLD contains two programs designed to get the tweets per second from the website [internet live stats](https://www.internetlivestats.com/one-second/), but after running the program for a few hours it was judged that the data was incorrect due to the absence of variation of the tweets per second over time. This was later proven correct when using the aforementioned method.

## Print to image
The printToImage program takes the location of a binary file with random numbers, reading them as 4 byte integers and outputs to a .ppm file that can be converted to a .png file.
The program plots the numbers on a 1000x1000 sized graph. The points that are changed follow from taking two random numbers X<sub>1</sub> and X<sub>2</sub> and adding a point at (X<sub>1</sub>, X<sub>2</sub>), this is then repeated for the next numbers with (X<sub>2</sub>, X<sub>3</sub>), (X<sub>x</sub>,X<sub>4</sub>)...
The program can take two arguments 'inputfile.bin', 'outputfile.ppm'
The .ppm file can be converted to a .png with the command
```
convert image.ppm -scale 400% image.png
```
Create all of the images for all binfiles with
```
make createImages
```

## Cleanup
To remove data files when done, run:
```
make clean
```
It will remove all files with that ends with .o .out .bin .ppm but not the downloaded tweets or results.
