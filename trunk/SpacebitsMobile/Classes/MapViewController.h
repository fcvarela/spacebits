
#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "AltairForwardPredictor.h"

@interface MapViewController : UIViewController <CLLocationManagerDelegate, MKMapViewDelegate> {
	NSNumber *altitude;
	NSNumber *distanceToAltair;
	AltairForwardPredictor *predictor;
	IBOutlet MKMapView *mapView;
}


@end

