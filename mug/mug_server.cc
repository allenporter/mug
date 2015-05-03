// MugService server implementation

#include <err.h>
#include <sysexits.h>
#include <iostream>
#include <vector>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>
#include "mug/mug_service.grpc.pb.h"
#include "mug/face_detector/face_detector.h"
#include <CoreFoundation/CFData.h>
#include <CoreGraphics/CGImage.h>
#include <ImageIO/CGImageSource.h>

using ::google::protobuf::RepeatedPtrField;

namespace mug {

bool FindFacesBoundingBox(CGImageRef image_ref,
    RepeatedPtrField<BoundingBox>* faces) {
  std::vector<FaceRect> face_rects;
  if (!DetectFaces(image_ref, &face_rects)) {
    return false;
  }
  for (FaceRect face_rect : face_rects) {
    BoundingBox* bounding_box = faces->Add();
    bounding_box->set_x1(face_rect.x1);
    bounding_box->set_y1(face_rect.y1);
    bounding_box->set_x2(face_rect.x2);
    bounding_box->set_y2(face_rect.y2);
  }
  return true;
}

class MugServiceImpl : public MugService::Service {
  grpc::Status DetectFaces(grpc::ServerContext* context,
      const DetectFacesRequest* request,
      DetectFacesResponse* response) {
    std::cout << "Request: " << request->image_bytes().size() << " bytes"
        << std::endl;
    CFDataRef data = CFDataCreate(NULL,
        (const UInt8*) request->image_bytes().data(),
        request->image_bytes().size());
    CGImageSourceRef source = CGImageSourceCreateWithData(data, NULL);
    CGImageRef image_ref = CGImageSourceCreateImageAtIndex(source, 0, NULL);
    if (!FindFacesBoundingBox(image_ref, response->mutable_faces())) {
      return grpc::Status(grpc::StatusCode::UNAVAILABLE,
          "Failed to detect faces");
    }
    return grpc::Status::OK;
  }
};

}

int main(int argc, char** argv) {
  if (argc < 2) {
    errx(EX_USAGE, "Usage: %s <port>", argv[0]);
  }
  grpc_init();

  const char* port = argv[1];
  char buf[BUFSIZ];
  snprintf(buf, BUFSIZ, "0.0.0.0:%s", port);
  std::string server_address(buf);
  mug::MugServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();  
  grpc_shutdown();
  return 0;
}
