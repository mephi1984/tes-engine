#import "ViewController.h"
#import "ios_api.h"


@implementation ViewController


- (void) appInitCaller
{
    CustomAppInit();
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}



@end