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

'''
Replicate the ign_tools command:

$ ign service -l
'''

from gz.transport import Node

def main():
    # create a transport node
    node = Node()

    # get list of services
    service_list = node.service_list()
    for service in service_list:
        print(service)

if __name__ == "__main__":
    main()

