#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "AltairAnnotation.h"
#import "AltairForwardPredictor.h"
#import "LocationController.h"

@interface MapViewController : UIViewController <LocationControllerDelegate, MKMapViewDelegate> {
	IBOutlet MKMapView *mapView;
	IBOutlet id distanceField;
	IBOutlet id bearingField;
	
	LocationController *locationController;
	AltairAnnotation *altairAnnotation;
	AltairForwardPredictor *altairForwardPredictorAnnotation;
	
	CLLocationCoordinate2D myPosition;
	CLLocationCoordinate2D altairPosition;
	float distance;
	float bearing;
}

- (void)locationUpdate:(CLLocation *)location; 
- (void)locationError:(NSError *)error;
- (void)recalcDistanceAndBearing;

- (float)estimateDistance:(CLLocationCoordinate2D)fromPoint toPoint:(CLLocationCoordinate2D)toPoint;
- (float)estimateCourse:(CLLocationCoordinate2D)fromPoint toPoint:(CLLocationCoordinate2D)toPoint;

@end

