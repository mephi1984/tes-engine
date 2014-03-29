//
//  Halibut_Engine.m
//  Halibut Engine
//
//  Created by vvv ооо on 13.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <UIKit/UIKit.h>
#include "include/Utils/Utils.h"


GLKView* defaultView = 0; //Need to be set up by App!

UITextField* extKeyboardTextView = 0;


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
    
void ShowKeyboard(std::string text)
{
    extKeyboardTextView.text = [NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding];
    [extKeyboardTextView becomeFirstResponder];
}
    
void HideKeyboard()
{
    [extKeyboardTextView resignFirstResponder];
}
    
void ClearKeyboardText()
{
    extKeyboardTextView.text = @"";
}
    
void GuiManagerSetKeyboardText(std::string newText); //Find it in GUIManager.cpp
    
void SetKeyboardText(const char* newText)
{
    SE::PerformInMainThreadAsync(boost::bind(&GuiManagerSetKeyboardText, newText));
    //GuiManagerSetKeyboardText(newText);
    //NSLog(@"text: %s", newText);
}
    
void GuiOnKeyboardHide(); //Find it in GUIManager.cpp
    
    
void OnKeyboardHide()
{
    SE::PerformInMainThreadAsync(&GuiOnKeyboardHide);
}
    
bool IsIpad()
{
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad )
    {
        return YES; /* Device is iPad */
    }
    return NO;
}
    
bool IsIphone5()
{
    if ([[UIScreen mainScreen] bounds].size.height == 568)
    {
        return YES; /* Device is iPhone5 */
    }
    return NO;
}
    


} //namespace SE

