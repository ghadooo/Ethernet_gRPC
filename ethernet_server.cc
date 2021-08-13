#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "viface/viface.hpp"

#include "etherInter.grpc.pb.h"
#include "etherInter.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using ethernet::InterfaceManager;
using ethernet::InfoRequest;
using ethernet::InfoReply;
using ethernet::setIPV4Request;
using ethernet::setDHCPRequest;
using ethernet::setStateRequest;
using ethernet::Empty;

// Logic and data behind the server's behavior.
class InterfaceManagerServiceImpl final : public InterfaceManager::Service {
  
  Status getInformation(
                  ServerContext* context, 
                  const InfoRequest* request,
                  InfoReply* reply) override {

    // libviface
    std::cout << "Got request for getInformation, interface: " << request->name() << "\n";
    viface::VIface iface(request->name());

    reply->set_mac(iface.getMAC());
    reply->set_ip(iface.getIPv4());
    reply->set_mask(iface.getIPv4Netmask());
    reply->set_state(iface.isUp());

    return Status::OK;
  }

    // sets the IPV4 
    Status setIPV4(ServerContext* context, const setIPV4Request* request, Empty* response) override {
      // libviface
      viface::VIface iface(request->name());
      return Status::OK;
    }
    // sets the network DHCP 
    Status setDHCP(ServerContext* context, const setDHCPRequest* request, Empty* response) override {
      // libviface
      viface::VIface iface(request->name());
      return Status::OK;
    }
    // sets the network state    
    Status setState(ServerContext* context, const setStateRequest* request, Empty* response) override {
      // libviface
      viface::VIface iface(request->name());
      return Status::OK;
    }

};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  InterfaceManagerServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}