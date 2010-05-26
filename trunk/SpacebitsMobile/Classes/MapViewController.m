#import "SpacebitsMobileAppDelegate.h"
#import "MapViewController.h"
#import "AltairAnnotation.h"
#import "JSON.h"

@implementation MapViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	
	altitude = 0;
	distanceToAltair = 0;
	
	predictor = [[AltairForwardPredictor alloc] init];
	
	CLLocationManager *locationManager=[[CLLocationManager alloc] init];
	locationManager.delegate=self;
	locationManager.desiredAccuracy=kCLLocationAccuracyNearestTenMeters;
	[locationManager startUpdatingLocation];
	
	[[[UIApplication sharedApplication] delegate] addObserver:self forKeyPath:@"telemetry" options:NSKeyValueObservingOptionNew context:nil];
	
	mapView.delegate = self;
}

- (MKAnnotationView *) mapView:(MKMapView *)mapView viewForAnnotation:(id <MKAnnotation>) annotation
{
	if ([annotation isKindOfClass:[MKUserLocation class]])
		return nil;
	
	MKPinAnnotationView *newAnnotation;
	if (annotation == predictor) {
		newAnnotation = [[MKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"green"];
		newAnnotation.pinColor = MKPinAnnotationColorGreen;
	} else {
		newAnnotation = [[MKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"redpin"];
		newAnnotation.pinColor = MKPinAnnotationColorRed;
	}
	newAnnotation.animatesDrop = YES;
	newAnnotation.canShowCallout = YES;
	
    return newAnnotation;
}


- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
	if ([keyPath isEqualToString:@"telemetry"]) {
		
		SpacebitsMobileAppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
		NSDictionary *telemetry = appDelegate.telemetry;
	
		AltairAnnotation *annot = [[[AltairAnnotation alloc] initWithLat:[[telemetry valueForKey:@"lat"] floatValue] lon:[[telemetry valueForKey:@"lon"] floatValue]] autorelease];
		[predictor didChangeCoordinates:annot.coordinate];
	
		NSArray *annotations = [[NSArray alloc] initWithArray:[mapView annotations]];
		NSMutableArray *delete = [[NSMutableArray alloc] initWithCapacity:1];
	
		for (id annot in annotations) {
			if ([annot isKindOfClass:[AltairAnnotation class]] || [annot isKindOfClass:[AltairForwardPredictor class]])
				[delete addObject: annot];
		}
	
		[mapView removeAnnotations:delete];
		[mapView addAnnotation:annot];
		[mapView addAnnotation:predictor];
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
