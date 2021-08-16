#include <iostream>
#include <memory>
#include <string>
#include <vector> 

#include <grpcpp/grpcpp.h>
#include "viface/viface.hpp"

#include "etherInter.grpc.pb.h"
#include "etherInter.pb.h"

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
      std::cout << "Status ok \n";
      const char * state = (reply.state() == true) ? "Up" : "Down";
      
      std::cout << "+------ " << name << " information ------+ \n";
      std::cout << "| MAC  : " << reply.mac() << "  |\n";
      std::cout << "| IPv4 : " << reply.ip() << "  |\n";
      std::cout << "| MASK : " << reply.mask() << "  |\n";
      std::cout << "| STATE: " << state << "  |\n";
      std::cout << "+--------------------------------+ \n";

    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
  }   


void setIPV4(std::string name, std::string ip, std::string mask) {

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;
    //std::cout << "Context created. \n";

    // Data we are sending to the server.
    setIPV4Request request;
    request.set_name(name);
    request.set_ip(ip);
    request.set_mask(mask);
    //std::cout << "Request created. \n";

    // Container for the data we expect from the server.
    Empty reply;
    //std::cout << "Reply created. \n";

    // The actual RPC.
    Status status = stub_->setIPV4(&context, request, &reply);
    //std::cout << "getInformation sent. \n";

    // Act upon its status.
    if (status.ok()) {
      std::cout << "Status ok \n";
      
      std::cout << "+------ Setting " << name << "'s IPv4 ------+ \n";
      //std::cout << "| MAC  : " << reply.mac() << "\n";
      std::cout << "| new IPv4 : " << ip << " |\n";
      std::cout << "| new MASK : " << mask << " |\n";
      //std::cout << "| STATE: " << state << "\n";
      std::cout << "+--------------------------------+ \n";

    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
  }   

void setState(std::string name, bool new_state) {

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;
    //std::cout << "Context created. \n";

    // Data we are sending to the server.
    setStateRequest request;
    request.set_name(name);
    request.set_state(new_state);
 
    //std::cout << "Request created. \n";

    // Container for the data we expect from the server.
    Empty reply;
    //std::cout << "Reply created. \n";

    // The actual RPC.
    Status status = stub_->setState(&context, request, &reply);
    //std::cout << "setState sent. \n";

    // Act upon its status.
    if (status.ok()) {
      std::cout << "Status ok \n";
      
      std::cout << "+------ Setting " << name << "'s state ------+ \n";
      //std::cout << "| MAC  : " << reply.mac() << "\n";
      //std::cout << "| new IPv4 : " << reply.ip() << "\n";
      //std::cout << "| new MASK : " << reply.mask() << "\n";
      std::cout << "| new STATE: " << new_state << "  |\n";
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

  infoClient.getInformation("wlp1s0");
  //infoClient.getInformation("enp2s0");

  std::cout << "+------ Further Settings  ------+ \n";
  std::cout << "\t| Setting IPv4    : 1   |\n";
  std::cout << "\t| Setting state   : 2   |\n";
  std::cout << "\t| Setting DHCP    : 3   |\n";
  std::cout << "\t| EXIT            : 4   |\n";
  std::cout << "+--------------------------------+ \n";

  int x;
  std::cin >> x ;
  switch (x)
  {
  case 1:
    {
      std::string new_ip, new_mask;
      std::cout << "\n Please enter new IPv4 : \t";
      std::cin >> new_ip;
      std::cout << "\n Please enter new mask : \t";
      std::cin >> new_ip;
      std::cout << "\n\n"; 
      infoClient.setIPV4("wlp1s0", new_ip, new_mask);
      break;
    }
    
  case 2:
      {
        bool new_state;
        std::cout << "\n Please enter new state : \t";
        std::cin >> new_state;
        std::cout << "\n\n"; 
        infoClient.setState("wlp1s0", new_state);
        break;
      }

  case 3:
      {
        std::string new_dhcp;
        std::cout << "\n Please enter new DHCP : \t";
        std::cin >> new_dhcp;
        std::cout << "\n\n"; 
        break;
      }

  case 4:
      {
        std::cout << "\n Exiting.....\t";
        sleep(1);
        exit(EXIT_SUCCESS);
        break;
      }

  default:
    std::cout << " Invalid choice! \n";
    break;
  }

  return 0;
}
