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

#include <ignition/msgs.hh>
#include <ignition/transport.hh>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "pybind11_protobuf/native_proto_caster.h"

#include <google/protobuf/message.h>

#include <functional>

namespace py = pybind11;

namespace ignition
{
namespace transport
{
namespace python
{
/// \brief Define pybind11 bindings for ignition::transport objects
void define_transport_node(py::module_ module)
{
  using namespace ignition;
  using namespace transport;

  pybind11_protobuf::ImportNativeProtoCasters();

  py::enum_<Scope_t>(module, "Scope_t")
    .value("PROCESS", Scope_t::PROCESS)
    .value("HOST", Scope_t::HOST)
    .value("ALL", Scope_t::ALL)
    ;

  py::class_<AdvertiseOptions>(
      module, "AdvertiseOptions")
      .def(py::init<>())
      .def_property("scope",
          &AdvertiseOptions::Scope,
          &AdvertiseOptions::SetScope)
      ;

  py::class_<AdvertiseMessageOptions, AdvertiseOptions>(
      module, "AdvertiseMessageOptions")
      .def(py::init<>())
      .def_property_readonly("throttled",
          &AdvertiseMessageOptions::Throttled)
      .def_property("msgs_per_sec",
          &AdvertiseMessageOptions::MsgsPerSec,
          &AdvertiseMessageOptions::SetMsgsPerSec)
      ;

  py::class_<AdvertiseServiceOptions, AdvertiseOptions>(
      module, "AdvertiseServiceOptions")
      .def(py::init<>())
      ;

  py::class_<SubscribeOptions>(
      module, "SubscribeOptions")
      .def(py::init<>())
      .def_property_readonly("throttled",
          &SubscribeOptions::Throttled)
      .def_property("msgs_per_sec",
          &SubscribeOptions::MsgsPerSec,
          &SubscribeOptions::SetMsgsPerSec)
      ;

  py::class_<Publisher>(
      module, "Publisher")
      .def(py::init<>())
      .def_property("topic",
          &Publisher::Topic,
          &Publisher::SetTopic)
      .def_property("addr",
          &Publisher::Addr,
          &Publisher::SetAddr)
      .def_property("puuid",
          &Publisher::PUuid,
          &Publisher::SetPUuid)
      .def_property("nuuid",
          &Publisher::NUuid,
          &Publisher::SetNUuid)
      // virtual
      .def_property("options",
          &Publisher::Options,
          &Publisher::SetOptions)
      // virtual
      .def("discovery", [](
          Publisher &_pub)
          {
            ignition::msgs::Discovery msg;
            _pub.FillDiscovery(msg);
            return msg;
          })
      ;

  py::class_<MessagePublisher, Publisher>(
      module, "MessagePublisher")
      .def(py::init<>())
      .def_property("ctrl",
          &MessagePublisher::Ctrl,
          &MessagePublisher::SetCtrl)
      .def_property("msg_type_name",
          &MessagePublisher::MsgTypeName,
          &MessagePublisher::SetMsgTypeName)
      // virtual
      .def_property("options",
          &MessagePublisher::Options,
          &MessagePublisher::SetOptions)
      // virtual
      .def("discovery", [](
          MessagePublisher &_pub)
          {
            ignition::msgs::Discovery msg;
            _pub.FillDiscovery(msg);
            return msg;
          })
      ;

  py::class_<ServicePublisher, Publisher>(
      module, "ServicePublisher")
      .def(py::init<>())
      .def_property("socket_id",
          &ServicePublisher::SocketId,
          &ServicePublisher::SetSocketId)
      .def_property("req_type_name",
          &ServicePublisher::ReqTypeName,
          &ServicePublisher::SetReqTypeName)
      .def_property("rep_type_name",
          &ServicePublisher::RepTypeName,
          &ServicePublisher::SetRepTypeName)
      // virtual
      .def_property("options",
          &ServicePublisher::Options,
          &ServicePublisher::SetOptions)
      // virtual
      .def("discovery", [](
          ServicePublisher &_pub)
          {
            ignition::msgs::Discovery msg;
            _pub.FillDiscovery(msg);
            return msg;
          })
      ;

  auto node = py::class_<Node>(module, "Node")
      .def(py::init<>())
      .def("advertise", static_cast<
          Node::Publisher (Node::*) (
              const std::string &,
              const std::string &,
              const AdvertiseMessageOptions &
          )>(&Node::Advertise),
          pybind11::arg("topic"),
          pybind11::arg("msg_type_name"),
          pybind11::arg("options"))
      .def("advertised_topics", &Node::AdvertisedTopics)
      .def("subscribe", [](
          Node &_node,
          const std::string &_topic,
          std::function<void(const google::protobuf::Message &_msg)> &_callback,
          const std::string &_msg_type_name,
          const SubscribeOptions &_opts)
          {
            return _node.Subscribe(_topic, _callback, _opts);
          },
          pybind11::arg("topic"),
          pybind11::arg("callback"),
          pybind11::arg("msg_type_name"),
          pybind11::arg("options"))
      .def("subscribed_topics", &Node::SubscribedTopics)
      .def("unsubscribe", &Node::Unsubscribe,
          pybind11::arg("topic"))
      .def("topic_list", [](
          Node &_node)
          {
            std::vector<std::string> topics;
            _node.TopicList(topics);
            return topics;
          })
      .def("topic_info", [](
          Node &_node,
          const std::string &_topic)
          {
            std::vector<MessagePublisher> publishers;
            _node.TopicInfo(_topic, publishers);
            return publishers;
          },
          pybind11::arg("topic"))
      .def("advertised_services", &Node::AdvertisedServices)
      // send a service request using the blocking interface
      .def("request", [](
          Node &_node,
          const std::string &_service,
          const google::protobuf::Message &_request,
          const unsigned int &_timeout,
          const std::string &_repType)
          {
            // see ign-transport/src/cmd/ign.cc L227-240
            auto rep = ignition::msgs::Factory::New(_repType);
            if (!rep)
            {
              std::cerr << "Unable to create response of type[" << _repType << "].\n";
              return false;
            }

            bool result{false};
            bool executed = _node.Request(_service, _request, _timeout, *rep, result);
            return (executed && result);
          },
          pybind11::arg("service"),
          pybind11::arg("request"),
          pybind11::arg("timeout"),
          pybind11::arg("rep_type_name"))
      .def("service_list", [](
          Node &_node)
          {
            std::vector<std::string> services;
            _node.ServiceList(services);
            return services;
          })
      .def("service_info", [](
          Node &_node,
          const std::string &_service)
          {
            std::vector<ServicePublisher> publishers;
            _node.ServiceInfo(_service, publishers);
            return publishers;
          },
          pybind11::arg("service"))
      ;

  // register Node::Publisher as a subclass of Node
  py::class_<ignition::transport::Node::Publisher>(node, "Publisher")
      .def(py::init<>())
      .def("valid", &ignition::transport::Node::Publisher::Valid)
      .def("publish", &ignition::transport::Node::Publisher::Publish,
          pybind11::arg("msg"))
      .def("throttled_update_ready",
          &ignition::transport::Node::Publisher::ThrottledUpdateReady)
      .def("has_connections",
          &ignition::transport::Node::Publisher::HasConnections)
      ;
}
}
}
}

/// \brief Define the ignition_transport module
PYBIND11_MODULE(transport, m) {
  pybind11_protobuf::ImportNativeProtoCasters();

  m.doc() = "Ignition Transport Python Library.";

  ignition::transport::python::define_transport_node(m);

  m.def("version", []() -> std::string {
    return IGNITION_TRANSPORT_VERSION_FULL;
  });

}
