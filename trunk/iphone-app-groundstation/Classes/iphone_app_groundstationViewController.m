//
//  iphone_app_groundstationViewController.m
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/17.
//  Copyright Caixa Mágica Software 2010. All rights reserved.
//

#import "iphone_app_groundstationViewController.h"
#import "AltairAnnotation.h"
#import "JSON.h"

@implementation iphone_app_groundstationViewController

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	
	altitude = 0;
	distanceToAltair = 0;
	
	CLLocationManager *locationManager=[[CLLocationManager alloc] init];
	locationManager.delegate=self;
	locationManager.desiredAccuracy=kCLLocationAccuracyNearestTenMeters;
	[locationManager startUpdatingLocation];
	
	responseData = [[NSMutableData data] retain];
	[NSTimer scheduledTimerWithTimeInterval:2.0 target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
	
}

- (void)onTimer
{
	[self spawnTelemetryRequest];
}



- (void)spawnTelemetryRequest
{
	NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://spacebits.eu/api/get"]];	
	[[NSURLConnection alloc] initWithRequest:request delegate:self];
	
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
	[responseData setLength:0];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
	[responseData appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
	
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
	[connection release];
	
	NSString *responseString = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
	NSDictionary *telemetry = [responseString JSONValue];
	
	return;AltairAnnotation *annot = [[[AltairAnnotation alloc] initWithLat:[[telemetry valueForKey:@"lat"] floatValue] lon:[[telemetry valueForKey:@"lon"] floatValue]] autorelease];
	
	[mapView addAnnotation:annot];
	NSArray *annotations = [mapView annotations];
	if ([annotations count] > 3) {
		NSArray *del = [annotations subarrayWithRange:NSMakeRange(0, [annotations count]-3)];
		[mapView removeAnnotations: del];
	}
}


- (IBAction)setMapType:(id)sender
{
	switch (((UISegmentedControl *)sender).selectedSegmentIndex) {
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


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
}


- (void)dealloc {
	[super dealloc];
}

@end
