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
import typing

from gz.msgs.stringmsg_pb2 import StringMsg

from gz.transport import SubscribeOptions
from gz.transport import Node

def cb(msg: StringMsg) -> None:
    print("Msg: [{}] from Python".format(msg.data))

def main():
    # create a transport node
    node = Node()
    topic = "/foo"
    msg_type_name = StringMsg.DESCRIPTOR.full_name
    sub_options = SubscribeOptions()

    # subscribe to a topic by registering a callback
    if node.subscribe(topic, cb, msg_type_name, sub_options):
        print("Subscribing to type {} on topic [{}]".format(
            msg_type_name, topic))
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

