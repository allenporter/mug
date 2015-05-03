PROTOC = "/usr/bin/protoc"
GRPC_PLUGIN = "/usr/bin/grpc_cpp_plugin"

genrule(
  name = "mug_service_proto",
  srcs = [ "mug_service.proto" ],
  outs = [
    "mug_service.pb.h",
    "mug_service.pb.cc",
    "mug_service.grpc.pb.h",
    "mug_service.grpc.pb.cc",
  ],
  cmd = PROTOC + " --cpp_out=$(GENDIR)/" \
        + " --grpc_out=$(GENDIR)/ --plugin=protoc-gen-grpc=" + GRPC_PLUGIN \
        + " $(location mug_service.proto)",
)

cc_library(
  name = "mug_service",
  srcs = [
    "mug_service.pb.cc",
    "mug_service.grpc.pb.cc",
  ],
  hdrs = [
    "mug_service.pb.h",
    "mug_service.grpc.pb.h",
  ],
  linkopts = [
    "-lprotobuf",
    "-lgrpc++",
  ],
)

cc_binary(
  name = "mug_client",
  srcs = [
    "mug_client.cc",
  ],
  deps = [
    ":mug_service",
  ],
  linkopts = [
    "-lgrpc",
  ],
)
