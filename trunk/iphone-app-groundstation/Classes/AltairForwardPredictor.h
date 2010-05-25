//
//  AltairForwardPredictor.h
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/25.
//  Copyright 2010 Caixa Mágica Software. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

@interface AltairForwardPredictor : NSObject <MKAnnotation> {
	CLLocationCoordinate2D coordinate;
	CLLocationCoordinate2D coordCache[3];
}

@property (nonatomic) CLLocationCoordinate2D coordinate;

- (void)didChangeCoordinates:(CLLocationCoordinate2D)newCoords;

@end
