#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>


@interface AltairAnnotation : NSObject <MKAnnotation> {
	NSString * title;
    NSString * subtitle;
	CLLocationCoordinate2D coordinate;
}

@property (nonatomic) CLLocationCoordinate2D coordinate;
@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *subtitle;

- (id)initWithLat:(float)lat lon:(float)lon;

@end
