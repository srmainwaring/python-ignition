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

import sys
import time

from gz.msgs.any_pb2 import Any
from gz.msgs.header_pb2 import Header
from gz.msgs.stringmsg_pb2 import StringMsg
from gz.msgs.time_pb2 import Time

from gz.msgs.extras import make_time
from gz.msgs.extras import take_time
from gz.msgs.extras import take_topic_info
from gz.msgs.extras import take_wrench

from gz.transport import AdvertiseMessageOptions
from gz.transport import Node


def main():
    # for item in sys.path:
    #   print(item)

    # msgs
    msg = Any()
    msg = Header()
    msg = StringMsg()

    # msgs functions
    msg = make_time()

    # transport types
    opts = AdvertiseMessageOptions()
    node = Node()
    pub = Node.Publisher()


if __name__ == "__main__":
    main()
