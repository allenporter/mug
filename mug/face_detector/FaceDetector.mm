#import "FaceDetector.h"
#include <mutex>

namespace {

std::once_flag once_init;
FaceDetector* faceDetector;

void Init() {
  faceDetector = [[FaceDetector alloc] init];
}

}  // namespace

@implementation FaceDetector

namespace mug {

bool DetectFaces(CGImageRef image, std::vector<mug::FaceRect>* face_rects) {
  call_once(once_init, Init);
  return [faceDetector detectFacesInImage:image withFaceRects:face_rects];
}

} // namespace mug


- (id) init {
  CIContext* context = [CIContext contextWithOptions:nil];
  NSDictionary* opts = @{ CIDetectorAccuracy : CIDetectorAccuracyHigh };
  ciDetector = [CIDetector detectorOfType:CIDetectorTypeFace
                                  context:context options:opts];
  return self;
}

- (bool) detectFacesInImage:(CGImageRef)imageRef
              withFaceRects:(std::vector<mug::FaceRect>*)faceRects {
  CIImage* image = [CIImage imageWithCGImage:(CGImageRef)imageRef];
  NSArray *features = [ciDetector featuresInImage:image options:nil];
  for (CIFaceFeature* f in features) {
    CGRect bounds = f.bounds;
    faceRects->push_back(mug::FaceRect(bounds.origin.x, bounds.origin.y,
        bounds.origin.x + (int) bounds.size.width,
        bounds.origin.y + (int) bounds.size.height));
  }
  return true;
}

@end
