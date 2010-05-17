//
//  iphone_app_groundstationViewController.m
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/17.
//  Copyright Caixa Mágica Software 2010. All rights reserved.
//

#import "iphone_app_groundstationViewController.h"

@implementation iphone_app_groundstationViewController


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	altitude = 0;
	distanceToAltair = 0;
	
	[super viewDidLoad];
}

-(IBAction)setMapType:(id)sender
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
