// A C++ wrapper around the MacOS face detection library

#ifndef __MUG_DETECTOR_H__
#define __MUG_DETECTOR_H__

#include <vector>
#include <mutex>
#include <CoreGraphics/CGImage.h>

namespace mug {

// Holds the coordinates of a face in an image (pixel coordinates)
struct FaceRect {
  int x1;
  int y1;
  int x2;
  int y2;

  FaceRect(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) { }
};

// Performs face detection on the specified image and populates the returned
// vector with a list of coordinates in the image that contain faces.
bool DetectFaces(CGImageRef image_ref, std::vector<FaceRect>* face_rects);

}  // namespace mug
  
#endif
