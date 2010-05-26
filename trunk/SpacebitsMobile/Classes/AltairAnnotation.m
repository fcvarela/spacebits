#import "AltairAnnotation.h"


@implementation AltairAnnotation

@synthesize coordinate;
@synthesize title;
@synthesize subtitle;
@synthesize view;

- (id)init
{
	self = [super init];
	title = @"Current Position";
	subtitle = @"";
	
	view = [[MKPinAnnotationView alloc] initWithAnnotation:self reuseIdentifier:@"red"];
	view.pinColor = MKPinAnnotationColorRed;
	view.canShowCallout = YES;
	
	return self;
}

@end
