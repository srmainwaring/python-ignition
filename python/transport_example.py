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

from gz.msgs.header_pb2 import Header
from gz.msgs.time_pb2 import Time
from gz.msgs.quaternion_pb2 import Quaternion
from gz.msgs.pose_pb2 import Pose
from gz.msgs.twist_pb2 import Twist
from gz.msgs.vector3d_pb2 import Vector3d
from gz.msgs.wrench_pb2 import Wrench

from gz.transport import AdvertiseMessageOptions
from gz.transport import SubscribeOptions
from gz.transport import Node

from google.protobuf.internal import api_implementation

def main():
    print("Ignition Transport Example\n")

    #----------------------------------------------
    # google.protobuf implementation
    print("proto api type: {}".format(api_implementation.Type()))

    #----------------------------------------------
    # create transport node
    node = Node()

    #----------------------------------------------
    # get a publisher and check valid
    pub = Node.Publisher()
    print("publisher valid: {}".format(pub.valid()))

    # create and publish different message types
    print("----------------------------------------")
    time_msg = Time()
    time_msg.sec = 15
    time_msg.nsec = 21
    pub.publish(time_msg)

    print("----------------------------------------")
    header_msg = Header()
    header_msg.stamp.CopyFrom(time_msg)  
    pub.publish(header_msg)

    print("----------------------------------------")
    vector3d_msg = Vector3d()
    vector3d_msg.header.CopyFrom(header_msg)
    vector3d_msg.x = 21.3
    vector3d_msg.y = 12.7
    vector3d_msg.z = 15.2
    pub.publish(vector3d_msg)

    print("----------------------------------------")
    quat_msg = Quaternion()
    quat_msg.header.CopyFrom(header_msg)
    quat_msg.x = 0.0
    quat_msg.y = 0.0
    quat_msg.z = 0.0
    quat_msg.w = 1.0
    pub.publish(quat_msg)

    print("----------------------------------------")
    pose_msg = Pose()
    pose_msg.header.CopyFrom(header_msg)
    pose_msg.name = "base_link"
    pose_msg.id = 0
    pose_msg.position.CopyFrom(vector3d_msg)
    pose_msg.orientation.CopyFrom(quat_msg)
    pub.publish(pose_msg)

    print("----------------------------------------")
    twist_msg = Twist()
    twist_msg.header.CopyFrom(header_msg)
    twist_msg.linear.CopyFrom(vector3d_msg)
    twist_msg.angular.CopyFrom(vector3d_msg)
    pub.publish(twist_msg)

    print("----------------------------------------")
    wrench_msg = Wrench()
    wrench_msg.header.CopyFrom(header_msg)
    wrench_msg.force.CopyFrom(vector3d_msg)
    wrench_msg.torque.CopyFrom(vector3d_msg)
    wrench_msg.force_offset.CopyFrom(vector3d_msg)
    pub.publish(wrench_msg)

    print("----------------------------------------")
    # AdvertiseMessageOptions defaults
    pub_options = AdvertiseMessageOptions()
    print("AdvertiseMessageOptions")
    print("throttled: {}".format(pub_options.throttled))
    print("msgs_per_sec: {}".format(pub_options.msgs_per_sec))
    # apply throttling
    pub_options.msgs_per_sec = 10
    print("throttled: {}".format(pub_options.throttled))
    print("msgs_per_sec: {}".format(pub_options.msgs_per_sec))

    topic = "/motor_1_joint/force_torque"
    msg_type_name = Wrench.DESCRIPTOR.full_name
    pub = node.advertise(topic, msg_type_name, pub_options)
    pub.publish(wrench_msg)

    print("----------------------------------------")
    def on_force_torque(msg):
        print(msg)

    sub_options = SubscribeOptions()
    print("SubscribeOptions")
    print("throttled: {}".format(sub_options.throttled))
    print("msgs_per_sec: {}".format(sub_options.msgs_per_sec))
    # sub = node.subscribe(topic, on_force_torque, sub_options)

if __name__ == "__main__":
    main()

