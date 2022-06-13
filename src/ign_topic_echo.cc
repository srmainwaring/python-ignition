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

#include <unistd.h>

#include <iostream>
#include <string>

#include <gz/msgs.hh>
#include <gz/transport.hh>

void cb(const google::protobuf::Message &_msg)
{
  std::cout << _msg.DebugString() << std::endl;
}

void print_usage()
{
  std::cout << "Must supply topic using '-t'" << std::endl;
}

int main(int argc, char **argv)
{
  // parse command line
  std::string topic;

  if (argc < 2)
  {
    print_usage();
    return -1;
  }
  int opt = 0;
  while ((opt = getopt(argc, argv, "t:")) != -1) 
  {
     switch (opt) 
     {
      case 't':
        topic = optarg;
        break;
      default:
        print_usage();
        return -1;
     }
  }

  // create node
  gz::transport::Node node;

  // subscribe to a topic by registering a callback.
  if (!node.Subscribe(topic, cb))
  {
    std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
    return -1;
  }

  gz::transport::waitForShutdown();
  return 0;
}
