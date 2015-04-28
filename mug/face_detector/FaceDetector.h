// A thin wrapper around the CIDetector API which is used by the C++ wrapper
// in face_detector.h.  This is not intended to be used directly.

#import <Foundation/Foundation.h>
#import <ImageIO/CGImageProperties.h>
#import <CoreImage/CoreImage.h>
#import "mug/face_detector/face_detector.h"

@interface FaceDetector : NSObject
{
  CIDetector* ciDetector;
}

- (bool) detectFacesInImage:(CGImageRef)imageRef
              withFaceRects:(std::vector<mug::FaceRect>*)faceRects;

@end
