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

using etherInter::InfoRequest;
using etherInter::InfoReply;
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

 
  // Container for the data we expect from the server.
    InfoReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->getInformation(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }   
  
  private:
   std::unique_ptr<InterfaceManager::Stub> stub_;  
};


int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  InterfaceManager info(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  vector<std::string> user(4);
  vector<std::string> reply;
  for (i(0); i<4 ; ++i) {
   reply.push_back(InterfaceManagerClient.getInformation(user(i)));
  std::cout << "Information received: " << reply(i) << std::endl; } 
  return 0;
}
