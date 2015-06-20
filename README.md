A grpc wrapper around the OS X face detection libraries.  That is, a grpc
face detection service.

This package is built with bazel and depends on rpc and protobuf.
protobuf: https://github.com/google/protobuf
grpc: https://github.com/grpc/grpc
bazel: http://bazel.io/docs/install.html

Use macports (https://www.macports.org) to help with pre-requisites of those
packages.

To build:
$ bazel build --ios_cpu=x86_64 --ios_sdk_version=8.3 //mug/...

Run the server:
$ bazel-bin/mug/mug_server 5050

Run a client:
$ bazel-bin/mug/mug_client localhost:5050 mug/face_detector/testdata/image.jpg
