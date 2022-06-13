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

from gz.msgs.cmd_vel2d_pb2 import CmdVel2D
from gz.msgs.double_pb2 import Double
from gz.msgs.double_v_pb2 import Double_V
from gz.msgs.float_pb2 import Float
from gz.msgs.float_v_pb2 import Float_V
from gz.msgs.header_pb2 import Header
from gz.msgs.pid_pb2 import PID
from gz.msgs.pose_pb2 import Pose
from gz.msgs.pose_v_pb2 import Pose_V
from gz.msgs.publish_pb2 import Publish
from gz.msgs.publishers_pb2 import Publishers
from gz.msgs.quaternion_pb2 import Quaternion
from gz.msgs.subscribe_pb2 import Subscribe
from gz.msgs.topic_info_pb2 import TopicInfo
from gz.msgs.time_pb2 import Time
from gz.msgs.twist_pb2 import Twist
from gz.msgs.vector3d_pb2 import Vector3d
from gz.msgs.wrench_pb2 import Wrench

from gz.transport import AdvertiseMessageOptions
from gz.transport import Node

def main():
    # Create a transport node and advertise options
    node = Node()
    opts = AdvertiseMessageOptions()

    # list of publishers
    publishers = []

    time_msg = Time()
    time_msg.sec = 15
    time_msg.nsec = 21
    publishers.append({
      "msg": time_msg,
      "pub": node.advertise(
          "/time", Time.DESCRIPTOR.full_name, opts)
    })

    header_msg = Header()
    header_msg.stamp.CopyFrom(time_msg)  
    publishers.append({
      "msg": header_msg,
      "pub": node.advertise(
          "/header", Header.DESCRIPTOR.full_name, opts)
    })

    double_msg = Double()
    double_msg.header.CopyFrom(header_msg)
    double_msg.data = 21.3
    publishers.append({
      "msg": double_msg,
      "pub": node.advertise(
          "/double", Double.DESCRIPTOR.full_name, opts)
    })

    double_v_msg = Double_V()
    double_v_msg.data.append(21.4)
    double_v_msg.data.append(12.8)
    double_v_msg.data.append(15.6)
    double_v_msg.data.append(10.1)
    publishers.append({
      "msg": double_v_msg,
      "pub": node.advertise(
          "/double_v", Double_V.DESCRIPTOR.full_name, opts)
    })

    float_msg = Float()
    float_msg.header.CopyFrom(header_msg)
    float_msg.data = 21.3
    publishers.append({
      "msg": float_msg,
      "pub": node.advertise(
          "/float", Float.DESCRIPTOR.full_name, opts)
    })

    float_v_msg = Float_V()
    float_v_msg.header.CopyFrom(header_msg)
    float_v_msg.data.append(21.4)
    float_v_msg.data.append(12.8)
    float_v_msg.data.append(15.6)
    float_v_msg.data.append(10.1)
    publishers.append({
      "msg": float_v_msg,
      "pub": node.advertise(
          "/float_v", Float_V.DESCRIPTOR.full_name, opts)
    })

    vector3d_msg = Vector3d()
    vector3d_msg.header.CopyFrom(header_msg)
    vector3d_msg.x = 21.3
    vector3d_msg.y = 12.7
    vector3d_msg.z = 15.2
    publishers.append({
      "msg": vector3d_msg,
      "pub": node.advertise(
          "/vector3d", Vector3d.DESCRIPTOR.full_name, opts)
    })

    quat_msg = Quaternion()
    quat_msg.header.CopyFrom(header_msg)
    quat_msg.x = 0.0
    quat_msg.y = 0.0
    quat_msg.z = 0.0
    quat_msg.w = 1.0
    publishers.append({
      "msg": quat_msg,
      "pub": node.advertise(
          "/quat", Quaternion.DESCRIPTOR.full_name, opts)
    })

    pose_msg = Pose()
    pose_msg.header.CopyFrom(header_msg)
    pose_msg.name = "base_link"
    pose_msg.id = 0
    pose_msg.position.CopyFrom(vector3d_msg)
    pose_msg.orientation.CopyFrom(quat_msg)

    pose_v_msg = Pose_V()
    pose_v_msg.header.CopyFrom(header_msg)

    pose_msg.name = "base_link"
    pose_msg.id = 0
    pose_v_msg.pose.append(pose_msg)

    pose_msg.name = "left_motor_link"
    pose_msg.id = 1
    pose_v_msg.pose.append(pose_msg)

    pose_msg.name = "right_motor_link"
    pose_msg.id = 2
    pose_v_msg.pose.append(pose_msg)
    publishers.append({
      "msg": pose_msg,
      "pub": node.advertise(
          "/pose", Pose.DESCRIPTOR.full_name, opts)
    })

    cmd_vel_msg = CmdVel2D()
    cmd_vel_msg.header.CopyFrom(header_msg)
    cmd_vel_msg.velocity = 8.25
    cmd_vel_msg.theta = 1.517
    publishers.append({
      "msg": cmd_vel_msg,
      "pub": node.advertise(
          "/cmd_vel", CmdVel2D.DESCRIPTOR.full_name, opts)
    })

    pid_msg = PID()
    pid_msg.header.CopyFrom(header_msg)
    double_msg.data = 1.25
    pid_msg.target_optional.CopyFrom(double_msg)
    double_msg.data = 0.5
    pid_msg.p_gain_optional.CopyFrom(double_msg)
    double_msg.data = 0.05
    pid_msg.i_gain_optional.CopyFrom(double_msg)
    double_msg.data = 0.005
    pid_msg.d_gain_optional.CopyFrom(double_msg)
    double_msg.data = 1.0
    pid_msg.i_max_optional.CopyFrom(double_msg)
    double_msg.data = -1.0
    pid_msg.i_min_optional.CopyFrom(double_msg)
    double_msg.data = 10.0
    pid_msg.limit_optional.CopyFrom(double_msg)
    publishers.append({
      "msg": pid_msg,
      "pub": node.advertise(
          "/pid", PID.DESCRIPTOR.full_name, opts)
    })

    twist_msg = Twist()
    twist_msg.header.CopyFrom(header_msg)
    twist_msg.linear.CopyFrom(vector3d_msg)
    twist_msg.angular.CopyFrom(vector3d_msg)
    publishers.append({
      "msg": twist_msg,
      "pub": node.advertise(
          "/twist", Twist.DESCRIPTOR.full_name, opts)
    })

    wrench_msg = Wrench()
    wrench_msg.header.CopyFrom(header_msg)
    wrench_msg.force.CopyFrom(vector3d_msg)
    wrench_msg.torque.CopyFrom(vector3d_msg)
    wrench_msg.force_offset.CopyFrom(vector3d_msg)
    publishers.append({
      "msg": wrench_msg,
      "pub": node.advertise(
          "/wrench", Wrench.DESCRIPTOR.full_name, opts)
    })

    pub_msg = Publish()
    pub_msg.header.CopyFrom(header_msg)
    pub_msg.topic = "/force_torque"
    pub_msg.msg_type = "gz.msgs.Wrench"
    pub_msg.host = "127.0.0.1"
    pub_msg.port = 11311
    publishers.append({
      "msg": pub_msg,
      "pub": node.advertise(
          "/publish", Publish.DESCRIPTOR.full_name, opts)
    })

    sub_msg = Subscribe()
    sub_msg.header.CopyFrom(header_msg)
    sub_msg.topic = "/force_torque"
    sub_msg.msg_type = "gz.msgs.Wrench"
    sub_msg.host = "127.0.0.1"
    sub_msg.port = 11311
    sub_msg.latching = True
    publishers.append({
      "msg": sub_msg,
      "pub": node.advertise(
          "/subscribe", Subscribe.DESCRIPTOR.full_name, opts)
    })

    topic_msg = TopicInfo()
    topic_msg.header.CopyFrom(header_msg)
    topic_msg.msg_type = "gz.msgs.Wrench"
    topic_msg.publisher.append(pub_msg)
    topic_msg.subscriber.append(sub_msg)
    publishers.append({
      "msg": topic_msg,
      "pub": node.advertise(
          "/topic_info", TopicInfo.DESCRIPTOR.full_name, opts)
    })

    # publish each of the items at 1Hz
    try:
        while True:
          for item in publishers:
            msg = item["msg"]
            pub = item["pub"]
            pub.publish(msg)

          time.sleep(1.0)

    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
