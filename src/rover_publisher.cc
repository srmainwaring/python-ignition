/*
 * Copyright (C) 2022 Rhys Mainwaring
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <gz/math/Quaternion.hh>
#include <gz/msgs.hh>
#include <gz/transport.hh>

static std::atomic<bool> g_terminatePub(false);

void signal_handler(int _signal)
{
  if (_signal == SIGINT || _signal == SIGTERM)
    g_terminatePub = true;
}

int main(int argc, char **argv)
{
  // Install a signal handler for SIGINT and SIGTERM.
  std::signal(SIGINT,  signal_handler);
  std::signal(SIGTERM, signal_handler);
  
  // Create a transport node and advertise a topic.
  gz::transport::Node node;

  std::string pose_topic = "/pose";
  auto pose_pub = node.Advertise<gz::msgs::Pose>(pose_topic);
  if (!pose_pub)
  {
    std::cerr << "Error advertising topic [" << pose_topic << "]\n";
    return -1;
  }

  std::string twist_topic = "/twist";
  auto twist_pub = node.Advertise<gz::msgs::Twist>(twist_topic);
  if (!twist_pub)
  {
    std::cerr << "Error advertising topic [" << twist_topic << "]\n";
    return -1;
  }

  // rover moving in a circle of radius with constant velocity
  double radius = 5.0;
  double ang_vel = 0.1;

  const auto start = std::chrono::steady_clock::now();

  // Publish messages at 2Hz.
  uint32_t count = 0;
  while (!g_terminatePub)
  {
    // update time
    const auto now = std::chrono::steady_clock::now();
    auto time_s = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
    auto time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count();
    time_ns = time_ns % 1000000000;
    uint32_t id = count++;

    // update position, orientation and velocity
    double wz = ang_vel;
    double theta = wz * time_s;
    double c = std::cos(theta);
    double s = std::sin(theta);
    double x = radius * c;
    double y = radius * s;
    double vx = -1.0 * radius * wz * s;
    double vy = radius * wz * c;
    gz::math::Quaternion quat(0.0, 0.0, theta);

    // Prepare the message.
    gz::msgs::Time time;
    time.set_sec(time_s);
    time.set_nsec(time_ns);

    gz::msgs::Header header;
    *header.mutable_stamp() = time;

    gz::msgs::Vector3d position;
    position.set_x(x);
    position.set_y(y);
    position.set_z(0.0);

    gz::msgs::Quaternion orientation;
    orientation.set_x(quat.X());
    orientation.set_y(quat.Y());
    orientation.set_z(quat.Z());
    orientation.set_w(quat.W());

    gz::msgs::Pose pose;
    pose.set_name("base_link");
    pose.set_id(id);
    *pose.mutable_header() = header;
    *pose.mutable_position() = position;
    *pose.mutable_orientation() = orientation;

    gz::msgs::Vector3d lin_vel_msg;
    lin_vel_msg.set_x(vx);
    lin_vel_msg.set_y(vy);
    lin_vel_msg.set_z(0.0);

    gz::msgs::Vector3d ang_vel_msg;
    ang_vel_msg.set_x(0.0);
    ang_vel_msg.set_y(0.0);
    ang_vel_msg.set_z(wz);

    gz::msgs::Twist twist;
    *twist.mutable_header() = header;
    *twist.mutable_linear() = lin_vel_msg;
    *twist.mutable_angular() = ang_vel_msg;

    if (!pose_pub.Publish(pose))
    {
      break;
    }
    if (!twist_pub.Publish(twist))
    {
      break;
    }
    std::cout << "Publishing pose on topic [" << pose_topic
        << "], twist on topic [" << twist_topic << "]\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0;
}
