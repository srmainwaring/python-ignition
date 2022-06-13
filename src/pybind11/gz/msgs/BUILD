load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

package(default_visibility = ["//visibility:public"])

#———————————————————————————————————————————————————————————————————————
# ignition_msgs_extras_lib
#
# TODO(srmainwaring) could move this back up to project root. 
# 
cc_library(
  name = "ignition_msgs_extras_lib",
  srcs = ["extras.cc"],
  deps = [
    "//python_ignition/include:ignition_msgs_extras_hdrs",
    "@ign-msgs9//:time_cc_proto",
    "@ign-msgs9//:topic_info_cc_proto",
    "@ign-msgs9//:wrench_cc_proto",
  ],
)

#———————————————————————————————————————————————————————————————————————
# ignition.msgs.extra
# 
# Python bindings for ignition msgs extras

pybind_extension(
  name = "extras",
  srcs = ["_ignition_msgs_extras_pybind11.cc"],
  deps = [
    ":ignition_msgs_extras_lib",
    "//python_ignition/include:ignition_msgs_extras_hdrs",
    "//ign_transport:ign_transport",
    "@pybind11_protobuf//pybind11_protobuf:native_proto_caster",
    "@com_google_protobuf//:protobuf",
  ],
)
