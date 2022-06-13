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

#include <iostream>
#include <string>
#include <gz/msgs.hh>
#include <gz/transport.hh>

void pose_cb(const gz::msgs::Pose &_msg)
{
  std::cout << _msg.DebugString();
}

void twist_cb(const gz::msgs::Twist &_msg)
{
  std::cout << _msg.DebugString();
}

int main(int argc, char **argv)
{
  gz::transport::Node node;

  // subscribe to pose
  std::string pose_topic = "/pose";
  if (!node.Subscribe(pose_topic, pose_cb))
  {
    std::cerr << "Error subscribing to topic ["
        << pose_topic << "]" << std::endl;
    return -1;
  }

  // subscribe to twist
  std::string twist_topic = "/twist";
  if (!node.Subscribe(twist_topic, twist_cb))
  {
    std::cerr << "Error subscribing to topic ["
        << twist_topic << "]" << std::endl;
    return -1;
  }

  // spin.
  gz::transport::waitForShutdown();
  return 0;
}
