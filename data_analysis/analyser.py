# This program analyses the raw data from the tweets collected in real time.
# It takes in a set amount of tweets, counts the amount of characters and the average for each tweet.
# It also checks for chains of repeating characters and bits and counts the different byte lengths.
# Furthermore it checks how the bits in each character changes, in relation to each other, from one to the next.
# See finalAnalysis for a large sampling of data, at the bottom of the file there is an array of the relations.
#
# Original code taken from twitter api documentation

import itertools
import os
import requests
import json
from pprint import pprint
from requests.auth import AuthBase
from requests.auth import HTTPBasicAuth

# This is where the keys obtained from twitter go
consumer_key = "" # Add your API key here
consumer_secret = "" # Add your API secret key here

stream_url = "https://api.twitter.com/labs/1/tweets/stream/sample"

numberOfTweetsToDownload = 100000

def insertionSort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i-1
        while j >= 0 and key[0] < arr[j][0]:
            arr[j+1] = arr[j]
            j -= 1
        arr[j+1] = key

# Gets a bearer token, used for authentication
class BearerTokenAuth(AuthBase):
  def __init__(self, consumer_key, consumer_secret):
    self.bearer_token_url = "https://api.twitter.com/oauth2/token"
    self.consumer_key = consumer_key
    self.consumer_secret = consumer_secret
    self.bearer_token = self.get_bearer_token()

  def get_bearer_token(self):
    response = requests.post(
      self.bearer_token_url,
      auth=(self.consumer_key, self.consumer_secret),
      data={'grant_type': 'client_credentials'},
      headers={"User-Agent": "TwitterDevSampledStreamQuickStartPython"})

    if response.status_code != 200:
      raise Exception(f"Cannot get a Bearer token (HTTP %d): %s" % (response.status_code, response.text))

    body = response.json()
    return body['access_token']

  def __call__(self, r):
    r.headers['Authorization'] = f"Bearer %s" % self.bearer_token
    return r

#this is the analyser
def stream_connect(auth, limit):
  #tracked variables
  numberOfChars = 0
  numberOfTweets = 0

  oneByteChars = 0
  twoByteChars = 0
  threeByteChars = 0
  fourByteChars = 0

  longestRepeatingCharChainLength = 0
  numRepeatingCharChain5 = 0
  numRepeatingCharsCount5 = 0
  numRepeatingCharChain10 = 0
  numRepeatingCharsCount10 = 0
  numRepeatingCharChain15Plus = 0
  numRepeatingCharsCount15plus = 0
  tweetsWithChain = 0

  arrSize = 0xEFbFbF
  arr = [[0, 'a']] * arrSize
  bitChanges = [0] * 32
  bitChain = [0] * 32
  bitChainFinal = [0] * 32
  bitPairChanges = []
  for i in range(32):
      row = []
      for j in range(32):
          row.append(0)
      bitPairChanges.append(row)
  prev = 0
  cur = 0

  # get the stream of tweets and do operations on them as they come in
  response = requests.get(stream_url, auth=auth, headers={"User-Agent": "TwitterDevSampledStreamQuickStartPython"}, stream=True)
  for response_line in response.iter_lines():
    if response_line:
        numberOfTweets += 1

        chain = 0
        chainFlag = False
        # get the text in the tweet and go through it character by character
        for c in json.loads(response_line)['data']['text']:
            numberOfChars += 1
            prev = cur
            byte = c.encode('utf-8')
            cur = int.from_bytes(byte, byteorder='big')

            # count the three byte characters and smaller, four bytes too big
            if cur < arrSize:
                if arr[cur][0] == 0:
                    arr[cur] = [1, c]
                else:
                    arr[cur][0] = arr[cur][0] + 1

            # count the bit changes
            toBitPair = [0] * 32
            for i in range(0, 32):
                if ((0x1 << i) & cur) != ((0x1 << i) & prev):
                    toBitPair[i] = 1
                    bitChanges[i] = bitChanges[i] + 1;
                    if bitChain[i] > bitChainFinal[i]:
                        bitChainFinal[i] = bitChain[i]
                    bitChain[i] = 0
                else:
                    bitChain[i] = bitChain[i] + 1
            for i in range(0,32):
                for j in range(0, 32):
                    if (toBitPair[i] & toBitPair[j]):
                        bitPairChanges[i][j] = bitPairChanges[i][j] + 1


            # search for character chains in the tweets
            if cur == prev:
                chain += 1
            elif chain < 5:
                chain = 1
            else:
                chainFlag = True
                if chain < 10:
                    numRepeatingCharChain5 += 1
                    numRepeatingCharsCount5 += chain
                elif chain < 15:
                    numRepeatingCharChain10 += 1
                    numRepeatingCharsCount10 += chain
                else:
                    numRepeatingCharChain15Plus += 1
                    numRepeatingCharsCount15plus += chain
                if chain > longestRepeatingCharChainLength:
# Listen to the stream. This reconnection logic will attempt to reconnect as soon as a disconnection is detected.ength:
                    longestRepeatingCharChainLength = chain
                chain = 1

            # count the amount of bytes the characters have
            if len(byte) == 1:
                oneByteChars += 1
            elif len(byte) == 2:
                twoByteChars += 1
            elif len(byte) == 3:
                threeByteChars += 1
            else:
                fourByteChars += 1

        # if the tweet ends in a chain
        if chain > 5:
            chainFlag = True
            if chain < 10:
                numRepeatingCharChain5 += 1
                numRepeatingCharsCount5 += chain
            elif chain < 15:
                numRepeatingCharChain10 += 1
                numRepeatingCharsCount10 += chain
            else:
                numRepeatingCharChain15Plus += 1
                numRepeatingCharsCount15plus += chain
            if chain > longestRepeatingCharChainLength:
                longestRepeatingCharChainLength = chain
        for i in range(0, 32):
            if bitChain[i] > bitChainFinal[i]:
                bitChainFinal[i] = bitChain[i]
        if chainFlag:
            tweetsWithChain += 1

        if numberOfTweets == limit:
            print('Number of tweets:', numberOfTweets)
            print('Number of characters:', numberOfChars)
            print('Average tweet length:', round(numberOfChars/numberOfTweets, 3), '\n')

            print('Chains longer than 5 smaller than 10:', numRepeatingCharChain5)
            print('Total characters:', numRepeatingCharsCount5)
            print('Percentage of total characters:', 100*round(numRepeatingCharsCount5/numberOfChars,3), '\n')

            print('Chains longer than 10 smaller than 15:', numRepeatingCharChain10)
            print('Total characters:', numRepeatingCharsCount10)
            print('Percentage of total characters:', 100*round(numRepeatingCharsCount10/numberOfChars,3), '\n')

            print('Chains longer than 15:', numRepeatingCharChain15Plus)
            print('Total characters:', numRepeatingCharsCount15plus)
            print('Percentage of total characters:', 100*round(numRepeatingCharsCount15plus/numberOfChars,3), '\n')

            print('Total amount of tweets with chains:', tweetsWithChain)
            print('Percentage of tweets:', 100*round(tweetsWithChain/numberOfTweets,3))
            print('Longest repeating chain', longestRepeatingCharChainLength)
            print('Total amount of chains:', numRepeatingCharChain5+numRepeatingCharChain10+numRepeatingCharChain15Plus)
            print('Total amount of chained characters:', numRepeatingCharsCount5+numRepeatingCharsCount10+numRepeatingCharsCount15plus)
            print('Percentage of total characters:', 100*round((numRepeatingCharsCount5+numRepeatingCharsCount10+numRepeatingCharsCount15plus)/numberOfChars, 3))


            print('')
            print('One Byte characters:', oneByteChars)
            print('Percetage of total characters:', 100*round(oneByteChars/numberOfChars, 3))
            print('Two Byte characters:', twoByteChars)
            print('Percetage of total characters:', 100*round(twoByteChars/numberOfChars, 3))
            print('Three Byte characters:', threeByteChars)
            print('Percetage of total characters:', 100*round(threeByteChars/numberOfChars, 3))
            print('Four Byte characters:', fourByteChars)
            print('Percetage of total characters:',100*round(fourByteChars/numberOfChars, 3) )

            print('')
            for i in range(0, 32):
                print('Bit', i, 'changes', bitChanges[i], 'times or', 100*round(bitChanges[i]/numberOfChars, 3), '% of the time')
            print('')
            for i in range(0, 32):
                print('Longest bit chain for bit', i, 'is',bitChainFinal[i])

            print('')
            newArraySize = 0
            for i in range(arrSize):
                if arr[i][0]:
                    newArraySize += 1

            newArray = [[0, '']] * newArraySize
            index = 0
            for i in range(arrSize):
                if arr[i][0]:
                    newArray[index] = arr[i]
                    index += 1

            insertionSort(newArray)
            newArray.reverse()
            for i in range(newArraySize):
                if newArray[i][0] != 0:
                    print(newArray[i][1], newArray[i][0])
            print('')
            for row in bitPairChanges:
                for val in row:
                    print(val, sep='\t', end=' ')
                print('')
            return

#program begins here
bearer_token = BearerTokenAuth(consumer_key, consumer_secret)
stream_connect(bearer_token, numberOfTweetsToDownload);
