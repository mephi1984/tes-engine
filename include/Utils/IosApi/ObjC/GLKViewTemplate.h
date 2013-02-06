//
//  CustomGLKView.h
//  doublehitballs
//
//  Created by vvv ооо on 15.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <GLKit/GLKit.h>

@interface GLKViewTemplate : GLKView
{
    
    CGPoint touchBeganLocation;
    
    bool touchMoved;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

@end
