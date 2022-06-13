#!/usr/bin/env python

# Copyright (C) 2022 Rhys Mainwaring
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

'''
Replicate the ign_tools command:

$ ign topic -i -e /topic
'''

import argparse
import time

from gz.transport import SubscribeOptions
from gz.transport import Node

# callback - prints the raw message
def cb(msg):
    print(msg)

def main():
    # process command line
    parser = argparse.ArgumentParser(description="Output data to screen.")
    parser.add_argument("-t", "--topic",
        metavar="topic", required=True, help="Name of a topic")
    args = parser.parse_args()

    # topic is required
    topic = args.topic

    # create a transport node
    node = Node()

    msg_type_name = "google.protobuf.Message"
    sub_options = SubscribeOptions()

    # subscribe to a topic by registering a callback
    if node.subscribe(topic, cb, msg_type_name, sub_options):
        print("Subscribing to topic [{}]".format(topic))
    else:
        print("Error subscribing to topic [{}]".format(topic))
        return

    # wait for shutdown
    try:
      while True:
        time.sleep(0.001)
    except KeyboardInterrupt:
      pass

if __name__ == "__main__":
    main()

