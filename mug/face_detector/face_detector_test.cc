// Reads a test image from disk, then runs face detection and verifies that
// it gets the correct number of faces.
// face_detector_test.cc

#include <err.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ImageIO/ImageIO.h>
#include <CoreGraphics/CoreGraphics.h>
#include "mug/face_detector/face_detector.h"

using std::vector;

#define TEST_IMAGE_FILE "mug/face_detector/testdata/image.jpg"

int main(int argc, const char * argv[])
{
  // Environment variable set by bazel
  const char* srcdir = getenv("TEST_SRCDIR");  
  if (srcdir == NULL) {
    fprintf(stderr, "Environment variable TEST_SRCDIR not set\n");
    return 1;
  }
  CFStringRef path = CFStringCreateWithFormat(NULL, NULL,
      CFSTR("file://%s/%s"), srcdir, TEST_IMAGE_FILE);
  CFURLRef url = CFURLCreateWithString(NULL, path, NULL);
  CGImageSourceRef source = CGImageSourceCreateWithURL(url, NULL);
  if (!source) {
    fprintf(stderr, "Can't create image source from url");
    return 1;
  }
  CGImageRef imageRef = CGImageSourceCreateImageAtIndex(source, 0, NULL);
  if (imageRef == NULL) {
    fprintf(stderr, "Failed to load image from path %s\n",
        CFStringGetCStringPtr(path, kCFStringEncodingMacRoman));
    return 1;
  }
  vector<mug::FaceRect> face_rects;
  if (!mug::DetectFaces(imageRef, &face_rects)) {
    fprintf(stderr, "Failed to detect faces\n");
    return 1;
  }
  if (face_rects.size() != 1) {
    fprintf(stderr, "Detected %lu faces instead of 1\n", face_rects.size());
    return 1;
  }
  for (mug::FaceRect face_rect : face_rects) {
    printf("Found face at (%d, %d) -> (%d, %d)\n", face_rect.x1, face_rect.y1,
        face_rect.x2, face_rect.y2);
  }
  CFRelease(url);
  CFRelease(source);
  CGImageRelease(imageRef);
  printf("PASS\n");
  return 0;
}

