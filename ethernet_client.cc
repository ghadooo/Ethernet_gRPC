#include <iostream>
#include <memory>
#include <string>
#include <vector> 

#include <grpcpp/grpcpp.h>
#include "viface/viface.hpp"

#include "etherInter.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using ethernet::InterfaceManager;
using ethernet::InfoRequest;
using ethernet::InfoReply;
using ethernet::setIPV4Request;
using ethernet::setDHCPRequest;
using ethernet::setStateRequest;
using ethernet::Empty;

class InterfaceManagerClient  {
 public:
 
   InterfaceManagerClient(std::shared_ptr<Channel> channel)
      : stub_(InterfaceManager::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.     
   void getInformation(std::string name) {

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;
    //std::cout << "Context created. \n";

    // Data we are sending to the server.
    InfoRequest request;
    request.set_name(name);
    //std::cout << "Request created. \n";

    // Container for the data we expect from the server.
    InfoReply reply;
    //std::cout << "Reply created. \n";

    // The actual RPC.
    Status status = stub_->getInformation(&context, request, &reply);
    //std::cout << "getInformation sent. \n";

    // Act upon its status.
    if (status.ok()) {
      //std::cout << "Status ok \n";
      const char * state = (reply.state() == true) ? "Up" : "Down";
      
      std::cout << "+------ " << name << " information ------+ \n";
      std::cout << "| MAC  : " << reply.mac() << "\n";
      std::cout << "| IPv4 : " << reply.ip() << "\n";
      std::cout << "| MASK : " << reply.mask() << "\n";
      std::cout << "| STATE: " << state << "\n";
      std::cout << "+--------------------------------+ \n";

    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
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
  InterfaceManagerClient infoClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  //std::cout << "Client created. \n";

  infoClient.getInformation("wlp3s0");
  infoClient.getInformation("enp2s0");

  return 0;
}
