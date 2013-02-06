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
    
    [self.view addGestureRecognizer:recognizer];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
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
