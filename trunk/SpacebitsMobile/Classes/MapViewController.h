#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "AltairAnnotation.h"
#import "AltairForwardPredictor.h"
#import "LocationController.h"

@interface MapViewController : UIViewController <LocationControllerDelegate, MKMapViewDelegate> {
	IBOutlet MKMapView *mapView;
	LocationController *locationController;
	AltairAnnotation *altairAnnotation;
	AltairForwardPredictor *altairForwardPredictorAnnotation;
}

- (void)locationUpdate:(CLLocation *)location; 
- (void)locationError:(NSError *)error;

@end

