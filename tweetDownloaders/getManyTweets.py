# Program that connects to the twitter api and gets a stream of recent tweets and prints them to standard output
#
# Original code taken from twitter api documentation


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

# here we connect to the stream and download the tweets
def stream_connect(auth, numTweets):
  response = requests.get(stream_url, auth=auth, headers={"User-Agent": "TwitterDevSampledStreamQuickStartPython"}, stream=True)
  for response_line in response.iter_lines():
    if response_line:
        numTweets -= 1
        reFlag = False
        #check if retweet, we want to remove the pattern "RT " at the beginning of these tweets
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

        # if chain found skip tweet, long chains gives bad random numbers
        if chainFlag or chain >= 5:
            continue
        #if retweet remove "RT " from beginning of text
        if (reFlag):
            print((json.loads(response_line)['data']['text'])[3:])
        else:
            print(json.loads(response_line)['data']['text'])
        if numTweets < 1:
            return

# program begins here, get authentication and dowload many tweets
bearer_token = BearerTokenAuth(consumer_key, consumer_secret)
stream_connect(bearer_token, 500000);
