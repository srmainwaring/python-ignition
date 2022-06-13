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

$ ign service -i -s /service
'''

import argparse

from gz.transport import Node

def main():
    # process command line
    parser = argparse.ArgumentParser(description="Get info about a service.")
    parser.add_argument("-s", "--service",
        metavar="service", required=True, help="Name of a service")
    args = parser.parse_args()

    # service is required
    service = args.service

    # create a transport node
    node = Node()

    # get list of service info
    service_info_list = node.service_info(service)

    # display address and message types
    print("Service providers [Address, Request Message Type, Response Message Type]:")
    for service_info in service_info_list:
        print("  {}, {}, {}".format(
            service_info.addr,
            service_info.req_type_name,
            service_info.rep_type_name))


if __name__ == "__main__":
    main()

