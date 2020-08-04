# Connects to twitter and gets a specified amount of tweets from a stream of recent tweets
#
# The program takes a number as argument when running it and will download that amount of tweets.
# If no argument is given, defaults to 100 tweets
#
# Original code taken from twitter api documentation


import os
import requests
import json
import sys
from pprint import pprint
from requests.auth import AuthBase
from requests.auth import HTTPBasicAuth

# This is where the keys obtained from twitter go
consumer_key = "" # Add your API key here
consumer_secret = "" # Add your API secret key here

stream_url = "https://api.twitter.com/labs/1/tweets/stream/sample"

# Gets a bearer token
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

def stream_connect(auth, numTweets):
  response = requests.get(stream_url, auth=auth, headers={"User-Agent": "TwitterDevSampledStreamQuickStartPython"}, stream=True)
  for response_line in response.iter_lines():
    if response_line:
        reFlag = False
        #check if retweet
        try:
            x = (json.loads(response_line)["data"]["referenced_tweets"])
            for tw in x:
                if tw['type'] == 'retweeted':
                    reFlag = True
        except:
            pass

        #check for chains in the string
        prev = 0
        cur = 0
        chain = 0
        chainFlag = False
        for c in json.loads(response_line)['data']['text']:
            prev = cur
            cur = ord(c)

            if cur == prev:
                chain += 1
            elif chain < 5:
                chain = 1
            else:
                chainFlag = True
                break

        # if chain found skip tweet
        if chainFlag or chain >= 5:
            continue
        #if retweet remove "RT " from beginning of text
        if (reFlag):
            print((json.loads(response_line)['data']['text'])[3:])
        else:
            print(json.loads(response_line)['data']['text'])

        numTweets = numTweets - 1
    if numTweets == 0:
        return


#program begins here, get authentication and download the specified or default amount of tweets
bearer_token = BearerTokenAuth(consumer_key, consumer_secret)
if len(sys.argv) < 2:
    stream_connect(bearer_token, 100);
else:
    stream_connect(bearer_token, int(sys.argv[1]));
