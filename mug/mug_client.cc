// Client library that sends RPCs to the mug service.
//
// Example usage
// $ mug_client <host:port> <filename.jpg>

#include <err.h>
#include <sysexits.h>
#include <iostream>
#include <fstream>
#include <grpc/grpc.h>
#include <grpc++/channel_arguments.h>
#include <grpc++/channel_interface.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/credentials.h>
#include <grpc++/status.h>
#include "mug/mug_service.grpc.pb.h"

using namespace std;

namespace {

// Reads the contents of the specified filename into the output buffer.
bool ReadBytesFromFile(const string& filename, string* out) {
  ifstream input_file(filename.c_str(), ios::in | ios::binary);
  if (!input_file.is_open()) {
    return false;
  }
  input_file.seekg(0, ios::beg);
  out->append(istreambuf_iterator<char>(input_file),
    istreambuf_iterator<char>());
  return true;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    errx(EX_USAGE, "Usage: %s <host:port> <image file> ...", argv[0]);
  }
  grpc_init();

  const char* host_port = argv[1];
  const char* filename = argv[2];

  shared_ptr<grpc::ChannelInterface> channel(
      grpc::CreateChannel(host_port, grpc::InsecureCredentials(),
          grpc::ChannelArguments()));
  unique_ptr<mug::MugService::Stub> stub(
      mug::MugService::NewStub(channel));
  mug::DetectFacesRequest request;
  if (!ReadBytesFromFile(filename, request.mutable_image_bytes())) {
    errx(EX_DATAERR, "Unable to open input file: %s", filename);
  }
  mug::DetectFacesResponse response;
  grpc::ClientContext context;
  grpc::Status status = stub->DetectFaces(&context, request, &response);
  if (!status.IsOk()) {
    cerr << "RPC failed: " << status.details() << endl;
    return 1;
  }
  cout << "Success: " << response.DebugString() << endl;
  channel.reset();
  stub.reset();

  grpc_shutdown();
}
