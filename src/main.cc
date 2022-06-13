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

#include <gz/msgs/time.pb.h>

#include "ignition/msgs/extras.hh"

int main(int argc, const char* argv[])
{
  // Verify the version of the library we linked against is
  // compatible with the version used to generate the headers.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::cout << "Ignition MsgsExtras Example" << std::endl;

  // example: gz/msgs/time.proto
  {
    gz::msgs::Time msg;
    msg.set_sec(11);
    msg.set_nsec(25);
    std::cout << msg.DebugString();
  }

  // example: msgs_tools
  {
    auto msg = gz::msgs::extras::MakeTime();
    gz::msgs::extras::TakeTime(msg);
  }

  // Shutdown
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
