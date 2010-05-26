#import "AltairAnnotation.h"


@implementation AltairAnnotation

@synthesize coordinate;
@synthesize title;
@synthesize subtitle;

- (id)initWithLat:(float)lat lon:(float)lon
{
	self = [super init];
	
	coordinate.latitude = lat;
	coordinate.longitude = lon;
	
	title = @"Current position";
    subtitle = @"Altitude";
	
	return self;
}

@end
