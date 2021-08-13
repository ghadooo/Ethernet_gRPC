#include <iostream>
#include <memory>
#include <string>
#include <vector> 

#include <grpcpp/grpcpp.h>
#include "viface/viface.hpp"

#ifdef BAZEL_BUILD    //not sure if this is the right format

#include "etherInter.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using etherInter::InfoRequest;
using etherInter::InfoReply;
using etherInter::Interface;
using etherInter::State;
using etherInter::InterfaceManager;

// Logic and data behind the server's behavior.
class InterfaceManagerServiceImpl final : public InterfaceManager::Service {
  Status getInformation(ServerContext* context, const InfoRequest* request,
                  InfoReply* reply) override {
    vector<std::string> prefix(4);
    prefix(0)="Name : ";
    prefix(1)="IPV4 : ";
    prefix(2)="DHCP : ";
    prefix(3)="State : ";
    vector<std::string> reply(4);
    for (i(0); i<4 ; ++i) {
    reply(i)->set_message(prefix(i) + request->name()); }
    return Status::OK;
  }
};
