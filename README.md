# Python Ignition

This project provides Python bindings for [`ignition-msgs`](https://github.com/ignitionrobotics/ign-msgs) and [`ignition-transport`](https://github.com/ignitionrobotics/ign-transport).

## Building with CMake

### Install Ignition

Follow the installation instructions for [Ignition Garden](https://ignitionrobotics.org/docs/garden).
This project depends directly on [`ignition-msgs`](https://github.com/ignitionrobotics/ign-msgs) and [`ignition-transport`](https://github.com/ignitionrobotics/ign-transport). These may be either available as system installs or in a source install in a local workspace folder which we assume is `~/workspace`.

### Install `python-ignition`

Clone this repo into the workspace source directory and update external submodules:

```bash
cd ~/workspace/src
git clone --recurse-submodules https://github.com/srmainwaring/python-ignition.git
```

### Build with CMake

Currently our use of [`pybind11_protobuf`](https://github.com/pybind/pybind11_protobuf) requires the protobuf compiler to generate the Python implementation of the Python protobuf bindings. This is configured with the environment variable:

```bash
export PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=python
```

Set the environment variable determing the Ignition version. This should be one of `edifice`, `fortress` or `garden`. The default is `garden`:

```bash
export IGNITION_VERSION=garden
```

Then create a build directory, configure and make:

```bash
mkdir -p ~/workspace/src/python-ignition/build
cd ~/workspace/src/python-ignition/build
cmake ..
make
```

### Update the Python environment

Update the PYTHONPATH to include the location of the extension modules and the
generated Python protobuf bindings.

```bash
cd ~/workspace/src/python-ignition
export PYTHONPATH=${PYTHONPATH}:$(pwd)/build/python
```

## Usage

### `ign-msg` bindings 

The Python bindings for `ign-msgs` are the standard generated protobuf code
for Python. For example `ign-msgs/proto/ignition/msgs/time.proto` may be used as follows: 

```python
# example.py
from ignition.msgs.time_pb2 import Time

msg = Time()
msg.sec = 15
msg.nsec = 21
print(msg)
```

### `ign-transport` bindings 

The Python bindings for `ign-transport` are contained in a module called `transport`.
The object naming and usage for the most part follows the C++ interface,
so the C++ Ignition Tutorials are a good guide on how to use the library.

Publish:

```python
from ignition.msgs.stringmsg_pb2 import StringMsg

from ignition.transport import AdvertiseMessageOptions
from ignition.transport import Node

# Create a transport node
node = Node()

# Advertise a topic
topic = "/foo"
msg_type_name = StringMsg.DESCRIPTOR.full_name
pub_options = AdvertiseMessageOptions()
pub = node.advertise(topic, msg_type_name, pub_options)

# Publish a message
msg = StringMsg()
msg.data = "hello"
pub.publish(msg)
```

Subscribe:

```python
import time
import typing

from ignition.msgs.stringmsg_pb2 import StringMsg

from ignition.transport import SubscribeOptions
from ignition.transport import Node

def cb(msg: StringMsg) -> None:
    print("Msg: [{}] from Python".format(msg.data))

# Create a transport node
node = Node()

# Subscribe to a topic by registering a callback
topic = "/foo"
msg_type_name = StringMsg.DESCRIPTOR.full_name
sub_options = SubscribeOptions()

node.subscribe(topic, cb, msg_type_name, sub_options)
```

## Examples

A number of examples in C++ and Python are provided. In the following we suppose that
they are being run from the project directory `~/workspace/src/python-ignition`.

---

`src/msg_example.cc` is a copy of the [`ign-msgs` tutorial example](https://ignitionrobotics.org/api/msgs/8.1/cppgetstarted.html):

```bash
$ ./build/msg_example
Point1:
x: 1
y: 3
z: 5

Point2:
x: 2
y: 4
z: 6
```

---

`src/publisher.cc` and `src/subscriber.cc` is copied from the [`ign-transport` messages tutorial example](https://ignitionrobotics.org/api/transport/11.0/messages.html).

From terminal 1:

```bash
$ ./build/publisher
Publishing hello on topic [/foo]
Publishing hello on topic [/foo]
...
```

From terminal 2:

```bash
$ ./build/subscriber
Msg: hello
Msg: hello
...
```

---

`src/rover_publisher.cc` and `src/rover_subscriber.cc` comprise another publish / subscribe
example that publishes the pose and twist of a rover moving in a circle with constant
angular velocity.

From terminal 1:

```bash
$ ./build/rover_publisher
Publishing pose on topic [/pose], twist on topic [/twist]
Publishing pose on topic [/pose], twist on topic [/twist]
...
```

From terminal 2:

```bash
$ ./build/rover_subscriber
header {
  stamp {
    sec: 10
    nsec: 45483925
  }
}
name: "base_link"
id: 20
position {
  x: 2.7015115293406988
  y: 4.2073549240394827
}
orientation {
  z: 0.479425538604203
  w: 0.87758256189037276
}
header {
  stamp {
    sec: 10
    nsec: 45483925
  }
}
linear {
  x: -0.42073549240394825
  y: 0.27015115293406988
}
angular {
  z: 0.1
}
```

---

`python/msgs_example.py` is a Python example that uses the generated Python protobuf libraries for `ign-msgs`:

```bash
$ ./python/msgs_example.py
Ignition Protobuf Example

proto api type: python
----------------------------------------
<class 'ignition.msgs.time_pb2.Time'>
sec: 15
nsec: 21
...
```

---

`python/publisher.py` is a Python version of the C++ `src/publisher.cc` described above.
You can listen to the messages using the C++ subscriber as before.

From terminal 1:

```bash
$ ./python/publisher.py 
Advertising ignition.msgs.StringMsg on topic [/foo]
Publishing hello on topic [/foo]
Publishing hello on topic [/foo]
...
```

From terminal 2:

```bash
$ ./python/subscriber.py 
Subscribing to type ignition.msgs.StringMsg on topic [/foo]
Msg: [hello] from Python
Msg: [hello] from Python
...
```

---

`python/rover_publisher.py` is a Python version of the C++ `src/rover_publisher.cc` example.

From terminal 1:

```bash
./python/rover_publisher.py
Advertising ignition.msgs.Pose on topic [/pose]
Advertising ignition.msgs.Twist on topic [/twist]
Publishing pose on topic [/pose], twist on topic [/twist]
Publishing pose on topic [/pose], twist on topic [/twist]
...
```

From terminal 2:

```bash
./python/rover_subscriber.py
Subscribing to type ignition.msgs.Pose on topic [/pose]
Subscribing to type ignition.msgs.Twist on topic [/twist]
header {
  stamp {
    sec: 2
    nsec: 511006000
  }
}
name: "base_link"
id: 5
position {
  x: 4.900332889206208
  y: 0.9933466539753061
}
orientation {
  z: 0.09983341664682815
  w: 0.9950041652780257
}

header {
  stamp {
    sec: 2
    nsec: 511006000
  }
}
linear {
  x: -0.09933466539753061
  y: 0.4900332889206208
}
angular {
  z: 0.1
}
...
```

--- 

`python/ign_topic_list.py` is a Python version of the command `ign topic -l`

```bash
$ ./python/ign_topic_list.py 
/foo
```

--- 

`python/ign_topic_info.py` is a Python version of the command `ign topic -i -t /foo`

```bash
$ ./python/ign_topic_info.py -t /foo
Publishers [Address, Message Type]:
  tcp://127.0.0.1:60328, ignition.msgs.StringMsg
```

--- 

`python/ign_topic_echo.py` is a Python version of the command `ign topic -e -t /foo`

```bash
$ ./python/ign_topic_echo.py -t /foo
Subscribing to topic [/foo]
data: "hello"

data: "hello"

data: "hello"
```

--- 

`python/ign_service_list.py` is a Python version of the command `ign service -l`

```bash
$ ./python/ign_service_list.py 
/gazebo/resource_paths/add
/gazebo/resource_paths/get
/gazebo/worlds
/gui/camera/view_control
/gui/follow
...
```

--- 

`python/ign_service_info.py` is a Python version of the command `ign service -i -s /gazebo/worlds`

```bash
$ ./python/ign_service_info.py -s /gazebo/worlds
Service providers [Address, Request Message Type, Response Message Type]:
  tcp://127.0.0.1:63657, ignition.msgs.Empty, ignition.msgs.StringMsg_V
```


## Building with Bazel

On macOS the project may also be built with Bazel. This is experimental and depends upon
a modified version of the Bazel build rules for Ignition in the [`ign-bazel`](https://github.com/ignitionrobotics/ign-bazel) project.


### Installing Bazel

On macOS Bazel can be installed with `brew`:

```bash
brew install bazel
```

The [Google protocol buffers compiler](https://github.com/protocolbuffers/protobuf) version `3.19` is also required and can be installed with:


```bash
brew install protobuf
```


### Setting up the workspace

Make a directory to contain this package and all the Ignition packages and dependencies:

```bash
mkdir ~/ignition/
cd ~/ignition/
```

Clone each of the packages using the `bazel.repos` file and [vcstool](https://github.com/dirk-thomas/vcstool). The forked version of [`ign-bazel`](https://github.com/srmainwaring/ign-bazel/tree/bazel-macos) contains modified build rules and repo references for macOS.

```bash
wget https://raw.githubusercontent.com/srmainwaring/ign-bazel/bazel-macos/example/bazel.repos
vcs import . < bazel.repos
```

Next, symlink the following files into the workspace directory:

```bash
cd ~/ignition
ln -sf ./ign_bazel/example/WORKSPACE.example ./WORKSPACE
ln -sf ./ign_bazel/example/BUILD.example ./BUILD.bazel
ln -sf ./ign_bazel/example/bazelrc.example ./.bazelrc
ln -sf ./python_ignition/ign-msgs9.BUILD ./ign-msgs9.BUILD
```

Finally, [`pybind11_protobuf`](https://github.com/pybind/pybind11_protobuf) requires a patch to the protobuf archive which must be available in the subdirectory `~/ignition/external`. It must be copied rather than symlinked:

```bash
cd ~/ignition
mkdir external
cp ./python_ignition/external/com_google_protobuf_build.patch external
```

### Build with Bazel

Currently our use of `pybind11_protobuf` requires the Python implementation of the Python protobuf generator. This is configured with the environment variable:  

```bash
export PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=python
```

Then build the examples with:

```bash
bazel build //python_ignition:all
bazel build //python_ignition/python:all
```

Note: on macOS not all Ignition projects will build with Bazel, so the command:

```bash
bazel build //... 
```

will result in a number of errors.

### Usage in Bazel builds

The Bazel build file `ign-msgs9.BUILD` defines targets for a selection of messages.
For example the targets for `proto/ignition/msgs/time.proto` are:

```bash
# proto_library
@ign-msgs9//:time_proto

# cpp_proto_library
@ign-msgs9//:time_cc_proto

# python_proto_library
@ign-msgs9//:time_py_pb2
```

To use the message bindings for C++ targets:

```bash
# BUILD.bazel

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
      "@ign-msgs9//:time_cc_proto",
  ],
)
```

To use the bindings for Python targets:

```bash
# BUILD.bazel

py_binary(
  name = "example",
  srcs = ["example.py"],
  data = [
    "@com_google_protobuf//:proto_api",
  ],
  deps = [
    "@ign-msgs9//:time_py_pb2",
  ],
)
```

## Notes

### Protobuf compiler version

The project depends on `protoc` version `3.19.1`. You must ensure that the version of `protoc` installed by `brew` matches (otherwise the examples will segfault).

```bash
$ protoc --version
libprotoc 3.19.1
```

### Protobuf generated Python libraries

The `brew` installation of `protobuf` defaults to use the C++ implementation
of the generated Python protobuf library. When using this with `pybind11_protobuf`
there are some cases when C++ wrapped protobuf objects returned from an extension module are not
recognised as their Python equivalents. The Python implementation will work in these cases
and to enable this set the following environment variable before building the project:

```bash
export PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=python
```

### CI Testing



### Repos used to build Ignition on macOS with Bazel

The table summarises the dependencies (repo and branch) on the Ignition libraries.

| library | repo | branch | build | test |
| --- | --- | --- | --- | --- |
| | | | | |
|python_ignition|https://github.com/srmainwaring/python-ignition|main|pass|-|
| | | | | |
|ign-bazel|https://github.com/srmainwaring/ign-bazel|bazel-macos|pass|pass|
|ign-math|https://github.com/srmainwaring/ign-math|bazel-macos/ign-math6|pass|pass|
|ign-utils|https://github.com/srmainwaring/ign-utils|bazel-macos/ign-utils1|pass|pass|
|ign-common|https://github.com/srmainwaring/ign-common|bazel-macos/ign-common4|pass|pass|
|ign-msgs|https://github.com/srmainwaring/ign-msgs|bazel-macos/ign-msgs9|pass|pass|
|sdformat|https://github.com/ignitionrobotics/sdformat| bazel-sdf10|pass|pass|
|ign-plugin|https://github.com/srmainwaring/ign-plugin|bazel-macos/ign-plugin1|pass|pass|
|ign-transport|https://github.com/srmainwaring/ign-transport|bazel-macos/ign-transport12|pass|1 fail|
|ign-physics|https://github.com/srmainwaring/ign-physics|bazel-macos/ign-physics5|pass|pass|
|ign-tools|https://github.com/srmainwaring/ign-tools|bazel-macos/ign-tools1|partial|-|
|ign-rendering|https://github.com/ignitionrobotics/ign-rendering|bazel-rendering4|fail|fail|
|ign-gui|https://github.com/ignitionrobotics/ign-gui|bazel-gui4|fail|fail|
|ign-sensors|https://github.com/ignitionrobotics/ign-sensors|bazel-sensors4|fail|fail|
|ign-gazebo|https://github.com/ignitionrobotics/ign-gazebo|bazel-gazebo4|fail|fail|
| | | | | |
