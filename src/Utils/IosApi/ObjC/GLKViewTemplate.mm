//
//  CustomGLKView.m
//  doublehitballs
//
//  Created by vvv ооо on 15.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "include/Utils/IosApi/ObjC/GLKViewTemplate.h"
#include "include/Utils/IosApi/IosWrapper.h"


#include <string>
namespace SE
{
void ShowKeyboard(std::string text);
}

@implementation GLKViewTemplate

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        touchMoved = false;
        
    }
    return self;
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    CGPoint location = [[[touches allObjects] objectAtIndex:0] locationInView:self];
    
    touchBeganLocation = location;
    
    touchMoved = false;
    
    SE::AppOnTapDown(location.x, self.bounds.size.height - location.y);

}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint location = [[[touches allObjects] objectAtIndex:0] locationInView:self];
    
    CGPoint prevLocation = [[[touches allObjects] objectAtIndex:0] previousLocationInView:self];
    
    if (abs(touchBeganLocation.x - location.x) > 10 || abs(touchBeganLocation.y - location.y) > 10)
    {
        touchMoved = true;
    }
    
    SE::AppOnScroll(prevLocation.x - location.x, -(prevLocation.y - location.y));
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint location = [[[touches allObjects] objectAtIndex:0] locationInView:self];
    
    if (touchMoved)
    {
        SE::AppOnTapUpAfterMove(location.x, self.bounds.size.height - location.y);
    }
    else
    {
        SE::AppOnTapUp(location.x, self.bounds.size.height - location.y);
    }
}


@end
