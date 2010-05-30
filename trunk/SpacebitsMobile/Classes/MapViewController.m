#import "SpacebitsMobileAppDelegate.h"
#import "MapViewController.h"
#import "AltairAnnotation.h"
#import "JSON.h"

#define r2d 57.29577951308
#define d2r 0.01745329252

#define pi 3.141592
#define pi2 6.283184

@implementation MapViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	
	locationController = [[LocationController alloc] init];
	locationController.delegate = self;
	[locationController.locationManager startUpdatingLocation];	
	
	[[[UIApplication sharedApplication] delegate] addObserver:self forKeyPath:@"telemetry" options:NSKeyValueObservingOptionNew context:nil];
	[[NSUserDefaults standardUserDefaults] addObserver:self forKeyPath:@"mapType" options:NSKeyValueObservingOptionNew context:nil];
	
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

- (void)locationUpdate:(CLLocation *)location {
	myPosition = location.coordinate;
	myPosition.latitude*=d2r;
	myPosition.longitude*=d2r;
	[self recalcDistanceAndBearing];
}

- (void)locationError:(NSError *)error {
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
	if ([keyPath isEqualToString:@"telemetry"]) {
		
		SpacebitsMobileAppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
		NSDictionary *telemetry = appDelegate.telemetry;
	
		CLLocationCoordinate2D newCoords;
		newCoords.latitude = [[telemetry valueForKey:@"lat"] floatValue];
		newCoords.longitude = [[telemetry valueForKey:@"lon"] floatValue];
		
		altairAnnotation.coordinate = newCoords;
		[altairForwardPredictorAnnotation didChangeCoordinates:newCoords];
		altairForwardPredictorAnnotation.coordinate = altairForwardPredictorAnnotation.coordinate;
		
		altairPosition = newCoords;
		altairPosition.latitude*=d2r;
		altairPosition.longitude*=d2r;
		[self recalcDistanceAndBearing];
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

- (void)recalcDistanceAndBearing
{
	NSLog(@"Calculating distance from: %f %f to %f %f",
		  myPosition.latitude, myPosition.longitude,
		  altairPosition.latitude, altairPosition.longitude);
	
	distance = [self estimateDistance:myPosition toPoint:altairPosition];
	bearing = [self estimateCourse:myPosition toPoint:altairPosition];
	
	bearing *= r2d;
	
	((UILabel *)distanceField).text = [NSString stringWithFormat:@"%.2f m", distance];
	((UILabel *)bearingField).text = [NSString stringWithFormat:@"%.1f deg", bearing];
}

- (float)estimateDistance:(CLLocationCoordinate2D)fromPoint toPoint:(CLLocationCoordinate2D)toPoint
{
	double t1 = sin(fromPoint.latitude)*sin(toPoint.latitude);
	double t2 = cos(fromPoint.latitude)*cos(toPoint.latitude);
	double t3 = cos(fromPoint.longitude-toPoint.longitude);
	double t4 = t2*t3;
	double t5 = t1+t4;
	double local_distance = acos( t5 );

	/* convert to meters */
	float distance_m = local_distance * 6366689.6;
	return distance_m;
}

- (float)estimateCourse:(CLLocationCoordinate2D)fromPoint toPoint:(CLLocationCoordinate2D)toPoint
{
	float course = fmod(atan2(sin(fromPoint.longitude-toPoint.longitude)*cos(toPoint.latitude),cos(fromPoint.latitude)*sin(toPoint.latitude)-sin(fromPoint.latitude)*cos(toPoint.latitude)*cos(fromPoint.longitude-toPoint.longitude)), pi2);

	// quick hack for symetric sign error. needs elegancy tweak later
	if(sin(toPoint.longitude - fromPoint.longitude) < 0.0)
		course = pi2 - course;
	if(course < 0.0)
		course = pi2-course;
	if(course > pi2)
		course = course-pi2;

	return(course);
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
