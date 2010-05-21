//
//  iphone_app_groundstationViewController.h
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/17.
//  Copyright Caixa Mágica Software 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

@interface iphone_app_groundstationViewController : UIViewController <CLLocationManagerDelegate> {
	NSNumber *altitude;
	NSNumber *distanceToAltair;
	NSMutableData *responseData;
	IBOutlet MKMapView *mapView;
}

- (IBAction)setMapType:(id)sender;
- (void)spawnTelemetryRequest;

@end

