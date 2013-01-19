//
//  CustomGLKView.m
//  doublehitballs
//
//  Created by vvv ооо on 15.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "CustomGLKView.h"
#import "ios_api.h"

bool touchMoved = false;
CGPoint prev_loc;


@implementation CustomGLKView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    CGPoint location = [[[touches allObjects] objectAtIndex:0] locationInView:self];
    
    prev_loc = location;
    
    touchMoved = false;
    
    AppOnTapDown(location.x, self.bounds.size.height - location.y);

}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint location = [[[touches allObjects] objectAtIndex:0] locationInView:self];
    
    CGPoint prevLocation = [[[touches allObjects] objectAtIndex:0] previousLocationInView:self];
    
    if (abs(prev_loc.x - location.x) > 10 || abs(prev_loc.y - location.y) > 10)
    {
        touchMoved = true;
    }
    
    AppOnScroll(prevLocation.x - location.x, prevLocation.y - location.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint location = [[[touches allObjects] objectAtIndex:0] locationInView:self];
    
    if (!touchMoved)
    {
        AppOnTapUp(location.x, self.bounds.size.height - location.y);
    }
}




@end
