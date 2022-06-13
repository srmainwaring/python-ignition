load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

package(default_visibility = ["//visibility:public"])

py_library(
  name = "foo_py",
  srcs = ["foo.py"],
)

#———————————————————————————————————————————————————————————————————————
# ignition.transport
# 
# Python bindings for ignition transport

pybind_extension(
  name = "transport",
  srcs = ["transport/_ignition_transport_pybind11.cc"],
  deps = [
    "//ign_transport:ign_transport",
    "@pybind11_protobuf//pybind11_protobuf:native_proto_caster",
    "@com_google_protobuf//:protobuf",
  ],
)
