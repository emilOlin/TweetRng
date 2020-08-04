import time
import datetime
import itertools
import os
import requests
import json
from pprint import pprint
from requests.auth import AuthBase
from requests.auth import HTTPBasicAuth

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

    if response.status_code is not 200:
      raise Exception(f"Cannot get a Bearer token (HTTP %d): %s" % (response.status_code, response.text))

    body = response.json()
    return body['access_token']

  def __call__(self, r):
    r.headers['Authorization'] = f"Bearer %s" % self.bearer_token
    return r

# connect to the server and get the tweets
def stream_connect(auth, runtime):
    count = 0
    response = requests.get(stream_url, auth=auth, headers={"User-Agent": "TwitterDevSampledStreamQuickStartPython"}, stream=True)
    start = time.time()
    for response_line in response.iter_lines():
        if response_line:
            count += 1
        if time.time() - start >= runtime:
            return count
    return count

# Program starts here
if os.path.exists("tdata.txt"):
    os.remove("tdata.txt")
f = open("tdata.txt", "w")
f.write("this file contains 7 days of measurements of the amout of tweets per second\n")
f.close()
for _ in itertools.repeat(None, 337): # every 30 minutes, 7 days
    start = time.time()
    bearer_token = BearerTokenAuth(consumer_key, consumer_secret)

    # Listen to the stream. This reconnection logic will attempt to run for 10 seconds and reconnect as soon as a disconnection is detected.
    runtimet = 30
    runtime = runtimet
    count = 0
    while True:
        t = time.time()
        count += stream_connect(bearer_token, runtime)
        if time.time() - t >= runtime:
            break
        else:
            runtime -= time.time() - t

    # write the date and time to a file together with data
    f = open("tdata.txt", "a")
    f.write(datetime.datetime.now().strftime("%Y/%m/%d, %H:%M:%S"))
    f.write("\n")
    f.write(str(count*100/runtimet))
    f.write("\n")
    f.close()

    # sleep for 30 minutes minus the time it took to iterate
    time.sleep(1800-(time.time() - start))
