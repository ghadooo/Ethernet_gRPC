#include <iostream>
#include <memory>
#include <string>
#include <vector> 

#include <grpcpp/grpcpp.h>
#include "viface/viface.hpp"

#ifdef BAZEL_BUILD    //not sure if this is the right format

#include "etherInter.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using etherInter::Interface;
using etherInter::State;
using etherInter::InterfaceManager;

class InterfaceManagerClient  {
 public:
 
   InterfaceManagerClient(std::shared_ptr<Channel> channel)
      : stub_(InterfaceManager::NewStub(channel)) {}
  // Assembles the client's payload, sends it and presents the response back
  // from the server.     
   vector<std::string> getInformation(const vector<std::string>& user) {
    // Data we are sending to the server.
    InfoRequest request;
    request.set_name(user.push_back(0));
    request.set_IP(user.push_back(1));
    request.set_DHCP(user.push_back(2));
    request.set_IP(user.push_back(3));

  /*  **************** first draft ***************
  
  // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }*/
  }   
  
   
}
