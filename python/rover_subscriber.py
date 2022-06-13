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

from gz.msgs.pose_pb2 import Pose
from gz.msgs.quaternion_pb2 import Quaternion
from gz.msgs.twist_pb2 import Twist
from gz.msgs.vector3d_pb2 import Vector3d

from gz.transport import Node
from gz.transport import SubscribeOptions


def pose_cb(msg: Pose) -> None:
    print("{}".format(msg))

def twist_cb(msg: Twist) -> None:
    print("{}".format(msg))

def main():
    # create a transport node
    node = Node()
    sub_options = SubscribeOptions()

    # subscribe to pose
    pose_topic = "/pose"
    pose_msg_type_name = Pose.DESCRIPTOR.full_name
    if node.subscribe(pose_topic, pose_cb, pose_msg_type_name, sub_options):
        print("Subscribing to type {} on topic [{}]".format(
            pose_msg_type_name, pose_topic))
    else:
        print("Error subscribing to topic [{}]".format(pose_topic))
        return

    # subscribe to twist
    twist_topic = "/twist"
    twist_msg_type_name = Twist.DESCRIPTOR.full_name
    if node.subscribe(twist_topic, twist_cb, twist_msg_type_name, sub_options):
        print("Subscribing to type {} on topic [{}]".format(
            twist_msg_type_name, twist_topic))
    else:
        print("Error subscribing to topic [{}]".format(twist_topic))
        return

    # wait for shutdown
    try:
      while True:
        time.sleep(0.001)
    except KeyboardInterrupt:
      pass

if __name__ == "__main__":
    main()

