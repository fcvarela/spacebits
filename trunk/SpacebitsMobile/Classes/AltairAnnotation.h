#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>


@interface AltairAnnotation : NSObject <MKAnnotation> {
	NSString * title;
    NSString * subtitle;
	CLLocationCoordinate2D coordinate;
	MKPinAnnotationView *view;
}

@property (nonatomic) CLLocationCoordinate2D coordinate;
@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *subtitle;
@property (nonatomic, retain) MKPinAnnotationView *view;

@end
