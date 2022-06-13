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

$ ign topic -i -t /topic
'''

import argparse

from gz.transport import Node

def main():
    # process command line
    parser = argparse.ArgumentParser(description="Get info about a topic.")
    parser.add_argument("-t", "--topic",
        metavar="topic", required=True, help="Name of a topic")
    args = parser.parse_args()

    # topic is required
    topic = args.topic

    # create a transport node
    node = Node()

    # get list of topic info
    topic_info_list = node.topic_info(topic)

    # display address and message type
    print("Publishers [Address, Message Type]:")
    for topic_info in topic_info_list:
        print("  {}, {}".format(topic_info.addr, topic_info.msg_type_name))


if __name__ == "__main__":
    main()

