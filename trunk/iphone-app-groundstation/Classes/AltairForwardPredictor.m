//
//  AltairForwardPredictor.m
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/25.
//  Copyright 2010 Caixa Mágica Software. All rights reserved.
//

#import "AltairForwardPredictor.h"


@implementation AltairForwardPredictor

@synthesize coordinate;

- (id)init
{
	self = [super init];
	return self;
}

- (void)didChangeCoordinates:(CLLocationCoordinate2D)newCoords
{
	NSLog(@"NEW %f %f", newCoords.latitude, newCoords.longitude);

	coordCache[0] = coordCache[1];
	coordCache[1] = coordCache[2];
	coordCache[2] = newCoords;
	
	// calculate deltas
	float latDelta1 = coordCache[1].latitude - coordCache[0].latitude;
	float latDelta2 = coordCache[2].latitude - coordCache[1].latitude;
	
	float lonDelta1 = coordCache[1].longitude - coordCache[0].longitude;
	float lonDelta2 = coordCache[2].longitude - coordCache[1].longitude;
	
	// forward propagate
	coordinate.latitude = newCoords.latitude + latDelta1*0.25 + latDelta2*0.75;
	coordinate.longitude = newCoords.longitude + lonDelta1*0.25 + lonDelta2*0.75;
}

@end
