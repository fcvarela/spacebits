#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

@interface AltairForwardPredictor : NSObject <MKAnnotation> {
	CLLocationCoordinate2D coordinate;
	CLLocationCoordinate2D coordCache[3];
	MKPinAnnotationView *view;
	NSString *title;
	NSString *subtitle;
	
	float distance, course;
}

@property (nonatomic) CLLocationCoordinate2D coordinate;
@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *subtitle;
@property (nonatomic, retain) MKPinAnnotationView *view;

- (void)didChangeCoordinates:(CLLocationCoordinate2D)newCoords;

@end
