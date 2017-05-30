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
#include <map>

struct TTouchHashData
{
    CGPoint first;
    bool second;
    int number;
};

std::map<UITouch*, TTouchHashData> touchHash;

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
        self.multipleTouchEnabled = YES;
        
    }
    return self;
}

- (void) AddTouchToHash:(UITouch*)touch
{
    touchHash[touch].first = [touch locationInView:self];
    touchHash[touch].second = false;
    
    for (int n = 0; n < 255; n++)
    {
        bool nExists = false;
        
        for (auto i = touchHash.begin(); i != touchHash.end(); ++i)
        {
            if (i->second.number == n)
            {
                nExists = true;
            }
        }
        
        if (nExists == false)
        {
            touchHash[touch].number = n;
            return;
        }
    }
}

- (void) RemoveTouchFromHash:(UITouch*)touch
{
    
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    //NSLog(@"touchesBegan");
    
    for (NSUInteger i = 0; i < [[touches allObjects] count]; i++)
    {
        UITouch *aTouch = [[touches allObjects] objectAtIndex:i];
        
        CGPoint location = [aTouch locationInView:self];
        
        [self AddTouchToHash:aTouch];
         
        SE::AppOnTapDown(location.x, self.bounds.size.height - location.y, touchHash[aTouch].number);
        
    }

}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (NSUInteger i = 0; i < [[touches allObjects] count]; i++)
    {
        UITouch *aTouch = [[touches allObjects] objectAtIndex:i];
        
        
        
        CGPoint location = [aTouch locationInView:self];
    
        CGPoint prevLocation = [aTouch previousLocationInView:self];
    
        if (abs(touchHash[aTouch].first.x - location.x) > 10 || abs(touchHash[aTouch].first.y - location.y) > 10)
        {
            touchHash[aTouch].second = true;
        }
    
        SE::AppOnScroll(prevLocation.x - location.x, -(prevLocation.y - location.y), touchHash[aTouch].number);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    //NSLog(@"touchesEnded");
    
    for (NSUInteger i = 0; i < [[touches allObjects] count]; i++)
    {
        UITouch *aTouch = [[touches allObjects] objectAtIndex:i];
        
    CGPoint location = [aTouch locationInView:self];
    
    if (touchHash[aTouch].second)
    {
        SE::AppOnTapUpAfterMove(location.x, self.bounds.size.height - location.y, touchHash[aTouch].number);
    }
    else
    {
        SE::AppOnTapUp(location.x, self.bounds.size.height - location.y, touchHash[aTouch].number);
    }
    touchHash.erase(aTouch);

    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    //NSLog(@"touchesCancelled");
    
    
    for (NSUInteger i = 0; i < [[touches allObjects] count]; i++)
    {
        UITouch *aTouch = [[touches allObjects] objectAtIndex:i];
        
        touchHash.erase(aTouch);
    }
}

@end
