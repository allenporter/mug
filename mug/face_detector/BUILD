FACE_DETECTOR_FRAMEWORKS = ["Foundation", "QuartzCore"]

objc_library(
  name = "face_detector_lib",
  srcs = [
    "FaceDetector.mm",
  ],
  hdrs = [
    "face_detector.h",
    "FaceDetector.h",
  ],
  sdk_frameworks = FACE_DETECTOR_FRAMEWORKS,
  visibility = ["//visibility:public"],
)

# Workaround until https://github.com/google/bazel/issues/161 is fixed
exports_files(["face_detector.h"])

cc_test(
  name = "face_detector_test",
  srcs = [
    "face_detector_test.cc",
  ],
  linkopts = [
    # This is a workaround until https://github.com/google/bazel/issues/161 is
    # fixed.
    "-L$(BINDIR)/mug/face_detector",
    "-lface_detector_lib",
    "-framework ImageIO",
    "-framework CoreGraphics",
    "-framework CoreFoundation",
  ] + ["-framework %s" % s for s in FACE_DETECTOR_FRAMEWORKS],
  data = [
    ":face_detector_lib",
    "//mug/face_detector/testdata:image.jpg",
  ],
)
