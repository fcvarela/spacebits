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
	
	NSString *title;
	NSString *subtitle;
}

@property (nonatomic) CLLocationCoordinate2D coordinate;
@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *subtitle;

- (void)didChangeCoordinates:(CLLocationCoordinate2D)newCoords;

@end
