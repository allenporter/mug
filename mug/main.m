//
//  main.m
//  mug
//
//  Created by Allen Porter on 1/1/14.
//  Copyright (c) 2014 Allen Porter. All rights reserved.
//

#include <err.h>
#include <sysexits.h>
#import <Foundation/Foundation.h>
#import <Foundation/NSURL.h>
#import <ImageIO/CGImageProperties.h>
#import <QuartzCore/CoreImage.h>

// Input key for CICrop that holds the vector for the crop rectangle
#define kCIInputRectangleKey @"inputRectangle"

int main(int argc, const char * argv[])
{
  @autoreleasepool {
    if (argc < 2) {
      errx(EX_USAGE, "Usage: %s <image file> ...", argv[0]);
    }
    
    CIContext* context = [CIContext contextWithCGContext:nil options:nil];
    NSDictionary* opts = @{ CIDetectorAccuracy : CIDetectorAccuracyHigh };
    CIDetector* detector = [CIDetector detectorOfType:CIDetectorTypeFace
                                              context:context options:opts];

    for (int i = 1; i < argc; ++i) {
      NSString* path = [NSString stringWithUTF8String:argv[i]];
      NSURL* url = [NSURL fileURLWithPath:path];
      
      CGImageSourceRef source = CGImageSourceCreateWithURL((__bridge CFURLRef) url, NULL);
      if (!source) {
        errx(EX_NOINPUT, "Error loading image source: %s was not found", argv[i]);
      }
      CGImageRef input = CGImageSourceCreateImageAtIndex(source, 0, NULL);
      CFStringRef imageType = CGImageSourceGetType(source);
      CIImage* image = [CIImage imageWithCGImage:(CGImageRef)input];
      NSArray *features = [detector featuresInImage:image options:nil];
      NSLog(@"Found %lu faces in %@", (unsigned long)features.count, url);

      CIFilter* crop = [CIFilter filterWithName:@"CICrop"];
      [crop setValue:image forKey:kCIInputImageKey];
      for (CIFaceFeature* f in features) {
        NSLog(@"CIFaceFeature: %@", NSStringFromRect(f.bounds));
        // TODO(aporter): Use a filname based on the original.  Make
        // sure it is different than the suffix used by iPhoto.
        // TODO(aporter): Use a configurable output directory.
        NSURL* outputUrl =
            [NSURL URLWithString:[NSString stringWithFormat:@"file:///tmp/X-%d", i]];
        CIVector* cropCoords = [CIVector vectorWithCGRect:f.bounds];
        NSLog(@"Crop coords: %@", cropCoords);
        [crop setValue:cropCoords forKey:kCIInputRectangleKey];
        CIImage *result = [crop valueForKey:kCIOutputImageKey];
        CGImageRef outputImage = [context createCGImage:result fromRect:[result extent]];
        CGImageDestinationRef dest = CGImageDestinationCreateWithURL(
            (__bridge CFURLRef) outputUrl, imageType, 1, NULL);
        CGImageDestinationAddImage(dest, outputImage, NULL);
        BOOL success = CGImageDestinationFinalize(dest);
        if (!success) {
          // TODO(aporter): Include filename in output
          errx(EX_IOERR, "Unable to write output file");
        }
        CFRelease(dest);
        CGImageRelease(outputImage);
      }
      
      CFRelease(imageType);
      CGImageRelease(input);
      CFRelease(source);
    }
  }
  return 0;
}

