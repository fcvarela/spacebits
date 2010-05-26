
#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "AltairAnnotation.h"
#import "AltairForwardPredictor.h"

@interface MapViewController : UIViewController <CLLocationManagerDelegate, MKMapViewDelegate> {
	IBOutlet MKMapView *mapView;
	AltairAnnotation *altairAnnotation;
	AltairForwardPredictor *altairForwardPredictorAnnotation;
}


@end

