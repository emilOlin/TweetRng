# gets the median of diffenrent char lengths from tweets

import itertools
import os


cur = 0
count = 0
arrForMedian1 = []
arrForMedian2 = []
arrForMedian3 = []
arrForMedian4 = []
data = open("manytweets", "r")
numberOfChars = 10**8
flag = False #flag to exit while loop

for line in data:
    for c in line:
        count += 1
        byte = c.encode('utf-8')
        cur = int.from_bytes(byte, byteorder='big')
        if len(byte) == 1:
            arrForMedian1.append(cur)
        elif len(byte) == 2:
            arrForMedian2.append(cur)
        elif len(byte) == 3:
            arrForMedian3.append(cur)
        elif len(byte) == 4:
            arrForMedian4.append(cur)
        if count >= numberOfChars:
            flag = True;
            break
    if flag:
        break

print(count, "characters")
arrForMedian1 = sorted(arrForMedian1)
print(arrForMedian1[int(len(arrForMedian1)/2)])
arrForMedian2 = sorted(arrForMedian2)
print(arrForMedian2[int(len(arrForMedian2)/2)])
arrForMedian3 = sorted(arrForMedian3)
print(arrForMedian3[int(len(arrForMedian3)/2)])
arrForMedian4 = sorted(arrForMedian4)
print(arrForMedian4[int(len(arrForMedian4)/2)])
