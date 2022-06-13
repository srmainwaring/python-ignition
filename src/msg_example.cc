/*
 * Copyright (C) 2014 Open Source Robotics Foundation
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

#include <gz/msgs.hh>

using namespace gz;

int main(int argc, const char *argv[])
{
  // https://gazebosim.org/api/msgs/8.1/cppgetstarted.html
  {
    gz::msgs::Vector3d point1;
    point1.set_x(1);
    point1.set_y(3);
    point1.set_z(5);
    gz::msgs::Vector3d point2;
    point2.set_x(2);
    point2.set_y(4);
    point2.set_z(6);
    std::cout << "Point1:\n" << point1.DebugString() << std::endl;
    std::cout << "Point2:\n" << point2.DebugString() << std::endl;
  }

  return 0;
}
