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

#include <gz/msgs.hh>
#include <gz/transport.hh>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "pybind11_protobuf/native_proto_caster.h"

#include <google/protobuf/message.h>

#include <functional>

namespace py = pybind11;

namespace gz
{
namespace transport
{
namespace python
{
/// \brief Define pybind11 bindings for gz::transport objects
void define_transport_node(py::module_ module)
{
  using namespace gz;
  using namespace transport;

  pybind11_protobuf::ImportNativeProtoCasters();

  py::enum_<Scope_t>(module, "Scope_t",
      "Defines the different options for the scope of a topic/service")
      .value("PROCESS", Scope_t::PROCESS,
          "Topic/service only available to subscribers in the"
          " same process as the publisher")
      .value("HOST", Scope_t::HOST,
          "Topic/service only available to subscribers in the"
          " same machine as the publisher")
      .value("ALL", Scope_t::ALL,
          "Topic/service available to any subscriber (default scope)")
      ;

  py::class_<AdvertiseOptions>(
      module, "AdvertiseOptions",
      "A class for customizing the publication options for"
      " a topic or service advertised")
      .def(py::init<>())
      .def_property("scope",
          &AdvertiseOptions::Scope,
          &AdvertiseOptions::SetScope,
          "The scope used in this topic/service")
      ;

  py::class_<AdvertiseMessageOptions, AdvertiseOptions>(
      module, "AdvertiseMessageOptions",
      "A class for customizing the publication options for a topic")
      .def(py::init<>())
      .def_property_readonly("throttled",
          &AdvertiseMessageOptions::Throttled,
          "Whether the publication has been throttled")
      .def_property("msgs_per_sec",
          &AdvertiseMessageOptions::MsgsPerSec,
          &AdvertiseMessageOptions::SetMsgsPerSec,
          "The maximum number of messages per second to be published")
      ;

  py::class_<AdvertiseServiceOptions, AdvertiseOptions>(
      module, "AdvertiseServiceOptions",
      "A class for customizing the publication options for a service")
      .def(py::init<>())
      ;

  py::class_<SubscribeOptions>(
      module, "SubscribeOptions",
      "A class to provide different options for a subscription")
      .def(py::init<>())
      .def_property_readonly("throttled",
          &SubscribeOptions::Throttled,
          "Whether the subscription has been throttled")
      .def_property("msgs_per_sec",
          &SubscribeOptions::MsgsPerSec,
          &SubscribeOptions::SetMsgsPerSec,
          "Set the maximum number of messages per second received per topic")
      ;

  py::class_<Publisher>(
      module, "Publisher",
      "This class stores all the information about a publisher."
      " It stores the topic name that publishes, addresses,"
      " UUIDs, scope, etc.")
      .def(py::init<>())
      .def_property("topic",
          &Publisher::Topic,
          &Publisher::SetTopic,
          "The topic published by this publisher")
      .def_property("addr",
          &Publisher::Addr,
          &Publisher::SetAddr,
          "Get the ZeroMQ address of the publisher")
      .def_property("puuid",
          &Publisher::PUuid,
          &Publisher::SetPUuid,
          "The process UUID of the publisher")
      .def_property("nuuid",
          &Publisher::NUuid,
          &Publisher::SetNUuid,
          "Get the node UUID of the publisher")
      // virtual
      .def_property("options",
          &Publisher::Options,
          &Publisher::SetOptions,
          "Get the advertised options")
      // virtual
      .def("discovery", [](
          Publisher &_pub)
          {
            gz::msgs::Discovery msg;
            _pub.FillDiscovery(msg);
            return msg;
          },
          "Populate a discovery message")
      ;

  py::class_<MessagePublisher, Publisher>(
      module, "MessagePublisher",
      "This class stores all the information about a message publisher")
      .def(py::init<>())
      .def_property("ctrl",
          &MessagePublisher::Ctrl,
          &MessagePublisher::SetCtrl,
          "The ZeroMQ control address. This address is used by the"
          " subscribers to notify the publisher about the new subscription")
      .def_property("msg_type_name",
          &MessagePublisher::MsgTypeName,
          &MessagePublisher::SetMsgTypeName,
          "Get the message type advertised by this publisher")
      // virtual
      .def_property("options",
          &MessagePublisher::Options,
          &MessagePublisher::SetOptions,
          "The advertised options")
      // virtual
      .def("discovery", [](
          MessagePublisher &_pub)
          {
            gz::msgs::Discovery msg;
            _pub.FillDiscovery(msg);
            return msg;
          },
          "Populate a discovery message")
      ;

  py::class_<ServicePublisher, Publisher>(
      module, "ServicePublisher",
      "This class stores all the information about a service publisher")
      .def(py::init<>())
      .def_property("socket_id",
          &ServicePublisher::SocketId,
          &ServicePublisher::SetSocketId,
          "The ZeroMQ socket ID for this publisher")
      .def_property("req_type_name",
          &ServicePublisher::ReqTypeName,
          &ServicePublisher::SetReqTypeName,
          "The name of the request's protobuf message advertised")
      .def_property("rep_type_name",
          &ServicePublisher::RepTypeName,
          &ServicePublisher::SetRepTypeName,
          "The name of the response's protobuf message advertised")
      // virtual
      .def_property("options",
          &ServicePublisher::Options,
          &ServicePublisher::SetOptions,
          "The advertised options")
      // virtual
      .def("discovery", [](
          ServicePublisher &_pub)
          {
            gz::msgs::Discovery msg;
            _pub.FillDiscovery(msg);
            return msg;
          },
          "Populate a discovery message")
      ;

  auto node = py::class_<Node>(module, "Node",
      "A class that allows a client to communicate with other peers."
      " There are two main communication modes: pub/sub messages"
      " and service calls")
      .def(py::init<>())
      .def("advertise", static_cast<
          Node::Publisher (Node::*) (
              const std::string &,
              const std::string &,
              const AdvertiseMessageOptions &
          )>(&Node::Advertise),
          pybind11::arg("topic"),
          pybind11::arg("msg_type_name"),
          pybind11::arg("options"),
          "Advertise a new topic. If a topic is currently advertised,"
          " you cannot advertise it a second time (regardless of its type)")
      .def("advertised_topics", &Node::AdvertisedTopics,
          "Get the list of topics advertised by this node")
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
          pybind11::arg("options"),
          "Subscribe to a topic registering a callback")
      .def("subscribed_topics", &Node::SubscribedTopics,
          "Get the list of topics subscribed by this node")
      .def("unsubscribe", &Node::Unsubscribe,
          pybind11::arg("topic"),
          "Unsubscribe from a topic")
      .def("topic_list", [](
          Node &_node)
          {
            std::vector<std::string> topics;
            _node.TopicList(topics);
            return topics;
          },
          "Get the list of topics currently advertised in the network")
      .def("topic_info", [](
          Node &_node,
          const std::string &_topic)
          {
            std::vector<MessagePublisher> publishers;
            _node.TopicInfo(_topic, publishers);
            return publishers;
          },
          pybind11::arg("topic"),
          "Get the information about a topic")
      .def("advertised_services", &Node::AdvertisedServices,
          "Get the list of services advertised by this node")
      // send a service request using the blocking interface
      .def("request", [](
          Node &_node,
          const std::string &_service,
          const google::protobuf::Message &_request,
          const unsigned int &_timeout,
          const std::string &_repType)
          {
            // see ign-transport/src/cmd/ign.cc L227-240
            auto rep = gz::msgs::Factory::New(_repType);
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
          pybind11::arg("rep_type_name"),
          "Request a new service without input parameter using"
          " a blocking call")
      .def("service_list", [](
          Node &_node)
          {
            std::vector<std::string> services;
            _node.ServiceList(services);
            return services;
          },
          "Get the list of topics currently advertised in the network")
      .def("service_info", [](
          Node &_node,
          const std::string &_service)
          {
            std::vector<ServicePublisher> publishers;
            _node.ServiceInfo(_service, publishers);
            return publishers;
          },
          pybind11::arg("service"),
          "Get the information about a service")
      ;

  // register Node::Publisher as a subclass of Node
  py::class_<gz::transport::Node::Publisher>(node, "Publisher",
      "A class that is used to store information about an"
      " advertised publisher.")
      .def(py::init<>())
      .def("valid", &gz::transport::Node::Publisher::Valid,
          "Return true if valid information, such as a non-empty"
          " topic name, is present.")
      .def("publish", &gz::transport::Node::Publisher::Publish,
          pybind11::arg("msg"),
          "Publish a message")
      .def("throttled_update_ready",
          &gz::transport::Node::Publisher::ThrottledUpdateReady,
          "")
      .def("has_connections",
          &gz::transport::Node::Publisher::HasConnections,
          "Return true if this publisher has subscribers")
      ;
}
}
}
}

/// \brief Define the gz_transport module
PYBIND11_MODULE(transport, m) {
  pybind11_protobuf::ImportNativeProtoCasters();

  m.doc() = "gz Transport Python Library.";

  gz::transport::python::define_transport_node(m);

  m.def("version", []() -> std::string {
    return GZ_TRANSPORT_VERSION_FULL;
  });

}
