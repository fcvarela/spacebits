#import "SpacebitsMobileAppDelegate.h"
#import "MapViewController.h"
#import "AltairAnnotation.h"
#import "JSON.h"

@implementation MapViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	
	CLLocationManager *locationManager=[[CLLocationManager alloc] init];
	locationManager.delegate=self;
	locationManager.desiredAccuracy=kCLLocationAccuracyNearestTenMeters;
	[locationManager startUpdatingLocation];
	
	[[[UIApplication sharedApplication] delegate] addObserver:self forKeyPath:@"telemetry" options:NSKeyValueObservingOptionNew context:nil];
	
	altairAnnotation = [[AltairAnnotation alloc] init];
	altairForwardPredictorAnnotation = [[AltairForwardPredictor alloc] init];
	
	[mapView addAnnotation:altairAnnotation];
	[mapView addAnnotation:altairForwardPredictorAnnotation];
	
	mapView.delegate = self;
}

- (MKAnnotationView *) mapView:(MKMapView *)mapView viewForAnnotation:(id <MKAnnotation>) annotation
{
	if ([annotation isKindOfClass:[MKUserLocation class]])
		return nil;
	
	MKAnnotationView *result;
	if (annotation == altairAnnotation)
		result = altairAnnotation.view;
	else
		result = altairForwardPredictorAnnotation.view;
	
	return result;
}


- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
	if ([keyPath isEqualToString:@"telemetry"]) {
		
		SpacebitsMobileAppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
		NSDictionary *telemetry = appDelegate.telemetry;
	
		CLLocationCoordinate2D newCoords;
		newCoords.latitude = [[telemetry valueForKey:@"lat"] floatValue];
		newCoords.longitude = [[telemetry valueForKey:@"lon"] floatValue];
		
		[altairAnnotation changeCoordinate:newCoords];
		
		///altairAnnotation.coordinate = newCoords;
		[altairForwardPredictorAnnotation didChangeCoordinates:newCoords];
		altairForwardPredictorAnnotation.coordinate = altairForwardPredictorAnnotation.coordinate;
	}
	
	if ([keyPath isEqualToString:@"mapType"]) {
		int mapType = [[[NSUserDefaults standardUserDefaults] valueForKey:@"mapType"] intValue];
		switch (mapType) {
			case 0:
				[mapView setMapType:MKMapTypeStandard];
				break;
			case 1:
				[mapView setMapType:MKMapTypeSatellite];
				break;
			case 2:
				[mapView setMapType:MKMapTypeHybrid];
				break;
		}
	}
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
}


- (void)dealloc {
	[super dealloc];
}


@end
