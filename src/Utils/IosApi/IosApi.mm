//
//  Halibut_Engine.m
//  Halibut Engine
//
//  Created by vvv ооо on 13.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#include "include/Utils/Utils.h"


GLKView* defaultView = 0; //Need to be set up by App!

namespace SE
{

std::string IosGetFileReadPath(const std::string& filename)
{
    @autoreleasepool
    {
    NSString* nsFullFileName = [NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding];
    
    NSString* nsFileShortName = [nsFullFileName lastPathComponent];
    
    NSString* nsFileShortNameWithoutExt = [nsFileShortName stringByDeletingPathExtension];
    
    NSString* nsFileExt = [nsFullFileName pathExtension];
    
    NSString* nsFilePath = [nsFullFileName stringByDeletingLastPathComponent];
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:nsFileShortNameWithoutExt ofType:nsFileExt inDirectory:nsFilePath];
    
    return std::string([filePath UTF8String]);
    }
}


std::string IosGetFilePathUserData(const std::string& filename)
{
    @autoreleasepool
    {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, 
                                                         NSUserDomainMask, YES);
    
    
    NSString* docDir = [paths objectAtIndex:0];
    docDir = [docDir stringByAppendingString:@"/"];
    
    NSString* file = [docDir stringByAppendingString:[NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding]];
    
    return std::string([[NSFileManager defaultManager] fileSystemRepresentationWithPath:file]);
    }
}
    
void IosSwitchToScreen()
{
    [defaultView bindDrawable];
}

} //namespace SE

