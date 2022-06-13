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

from scipy.spatial.transform import Rotation as Rotation
import math
import time

from gz.msgs.header_pb2 import Header
from gz.msgs.pose_pb2 import Pose
from gz.msgs.quaternion_pb2 import Quaternion
from gz.msgs.time_pb2 import Time
from gz.msgs.twist_pb2 import Twist
from gz.msgs.vector3d_pb2 import Vector3d

from gz.transport import AdvertiseMessageOptions
from gz.transport import Node

def main():
    # Create a transport node and advertise a topic
    node = Node()
    pub_options = AdvertiseMessageOptions()

    pose_topic = "/pose"
    pose_msg_type_name = Pose.DESCRIPTOR.full_name

    pose_pub = node.advertise(
        pose_topic, pose_msg_type_name, pub_options)
    if pose_pub.valid():
        print("Advertising {} on topic [{}]".format(
            pose_msg_type_name, pose_topic))
    else:
        print("Error advertising topic [{}]".format(pose_topic))

    twist_topic = "/twist"
    twist_msg_type_name = Twist.DESCRIPTOR.full_name
    twist_pub = node.advertise(
        twist_topic, twist_msg_type_name, pub_options)
    if twist_pub.valid():
        print("Advertising {} on topic [{}]".format(
            twist_msg_type_name, twist_topic))
    else:
        print("Error advertising topic [{}]".format(twist_topic))

    # rover moving in a circle of radius with constant velocity
    radius = 5.0
    ang_vel = 0.1

    # publish messages at 2Hz
    start = time.time_ns()
    count = 0
    try:
        while True:
            # update time
            now = time.time_ns()
            time_ns = now - start
            time_s = int(time_ns/1.0E9)
            time_ns = int(time_ns % 1000000000)
            id = count
            count += 1

            # update position, orientation and velocity
            wz = ang_vel
            theta = wz * time_s
            c = math.cos(theta)
            s = math.sin(theta)
            x = radius * c
            y = radius * s
            vx = -1.0 * radius * wz * s
            vy = radius * wz * c
            rot = Rotation.from_euler("xyz", [0.0, 0.0, theta])
            quat = rot.as_quat()

            # # Prepare the messages.
            time_msg = Time()
            time_msg.sec = time_s
            time_msg.nsec = time_ns

            header = Header()
            header.stamp.CopyFrom(time_msg)

            position = Vector3d()
            position.x = x
            position.y = y
            position.z = 0.0

            orientation = Quaternion() 
            orientation.x = quat[0]
            orientation.y = quat[1]
            orientation.z = quat[2]
            orientation.w = quat[3]

            pose = Pose()
            pose.name = "base_link"
            pose.id = id
            pose.header.CopyFrom(header)
            pose.position.CopyFrom(position)
            pose.orientation.CopyFrom(orientation)

            lin_vel_msg = Vector3d() 
            lin_vel_msg.x = vx
            lin_vel_msg.y = vy
            lin_vel_msg.z = 0.0

            ang_vel_msg = Vector3d() 
            ang_vel_msg.x = 0.0
            ang_vel_msg.y = 0.0
            ang_vel_msg.z = wz

            twist = Twist()
            twist.header.CopyFrom(header)
            twist.linear.CopyFrom(lin_vel_msg)
            twist.angular.CopyFrom(ang_vel_msg)

            if not pose_pub.publish(pose):
                break

            if not twist_pub.publish(twist):
                break

            print("Publishing pose on topic [{}], twist on topic [{}]".format(
                pose_topic, twist_topic))

            time.sleep(0.5)

    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()

