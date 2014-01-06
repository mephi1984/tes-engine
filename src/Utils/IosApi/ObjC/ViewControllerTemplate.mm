//
//  ViewController.m
//  doublehitballs
//
//  Created by vvv ооо on 13.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "include/Utils/IosApi/ObjC/ViewControllerTemplate.h"
#include "include/Utils/IosApi/IosWrapper.h"


extern GLKView* defaultView; //Find this in IosApi.mm

extern UITextField* extKeyboardTextView; //Find this in IosApi.mm

namespace SE
{

void SetKeyboardText(const char* newText);
void OnKeyboardHide();

}

@interface ViewControllerTemplate () {
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewControllerTemplate

@synthesize context = _context;
@synthesize effect = _effect;
@synthesize hiddenTextField = _hiddenTextField;


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    defaultView = view;
    
    [self setupGL];
    
    
    
    //Gesture Recognizers
    
    UIPinchGestureRecognizer *recognizer = [[UIPinchGestureRecognizer alloc]
                                            initWithTarget:self action:@selector(respondToPinch:)];
    
    
    recognizer.delaysTouchesEnded = NO;
    
    [self.view addGestureRecognizer:recognizer];
    
    self.view.multipleTouchEnabled = YES;
    
    //Keyboard
    
    _hiddenTextField = [[UITextField alloc] initWithFrame:CGRectMake(-200,-200,0,0)];
    
    //[_hiddenTextField setReturnKeyType: UIReturnKeyDone];
    
    _hiddenTextField.autocorrectionType = UITextAutocorrectionTypeNo;
    
    [self.view addSubview:_hiddenTextField];
    
    extKeyboardTextView = _hiddenTextField;
    
    _hiddenTextField.delegate = self;
    
     [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onReceiveKeyboardNotification:) name:UITextFieldTextDidChangeNotification object:nil];
    
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
    
    _hiddenTextField = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    [self appInitCaller];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    SE::AppDeinit();
    
}

- (void) appInitCaller
{
}

- (void)respondToPinch:(UIPinchGestureRecognizer *)gestureRecognizer
{
    SE::AppOnScale(gestureRecognizer.scale);
}

- (void) onReceiveKeyboardNotification:(NSNotification*) notification
{
    if (notification.name == UITextFieldTextDidChangeNotification)
    {
        UITextField* textField = notification.object;
        const char* text = [[textField text] UTF8String];
        SE::SetKeyboardText(text);
    }
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    NSLog(@"Begin");
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    NSLog(@"End");
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if([string isEqualToString:@"\n"])
    {
        [textField resignFirstResponder];
        SE::OnKeyboardHide();
    }
    return YES;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    
    SE::AppUpdate(self.timeSinceLastUpdate * 1000);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    
    SE::AppDraw();
    
}

@end

