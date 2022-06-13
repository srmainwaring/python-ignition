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

import time

from gz.msgs.stringmsg_pb2 import StringMsg

from gz.transport import AdvertiseMessageOptions
from gz.transport import Node

def main():
    # Create a transport node and advertise a topic
    node = Node()
    topic = "/foo"
    msg_type_name = StringMsg.DESCRIPTOR.full_name
    pub_options = AdvertiseMessageOptions()
    pub = node.advertise(topic, msg_type_name, pub_options)
    if pub.valid():
        print("Advertising {} on topic [{}]".format(msg_type_name, topic))
    else:
        print("Error advertising topic [{}]".format(topic))

    # Prepare the message
    msg = StringMsg()
    msg.data = "hello"

    try:
        while True:
          if not pub.publish(msg):
              break

          print("Publishing hello on topic [{}]".format(topic))
          time.sleep(1.0)

    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()

