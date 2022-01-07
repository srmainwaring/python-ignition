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
#include <ignition/msgs.hh>
#include <ignition/transport.hh>

void twist_cb(const ignition::msgs::Twist &_msg)
{
  std::cout << _msg.DebugString();
}

int main(int argc, char **argv)
{
  ignition::transport::Node node;
  std::string topic = "/twist";

  // Subscribe to a topic by registering a callback.
  if (!node.Subscribe(topic, twist_cb))
  {
    std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
    return -1;
  }

  // spin.
  ignition::transport::waitForShutdown();
  return 0;
}