//
//  ViewController.m
//  doublehitballs
//
//  Created by vvv ооо on 13.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"
#import "ios_api.h"

@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

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
    
    
    [self setupGL];
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

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    /*
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }*/
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    AppInit();
    }

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    AppDeinit();
    
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    
    AppUpdate(self.timeSinceLastUpdate * 1000);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    
    AppDraw();
    
}


@end
