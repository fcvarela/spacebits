//
//  AltairAnnotation.h
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/19.
//  Copyright 2010 Caixa Mágica Software. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>


@interface AltairAnnotation : NSObject <MKAnnotation> {
	NSString * title;
    NSString * subtitle;
	CLLocationCoordinate2D coordinate;
}

@property (nonatomic) CLLocationCoordinate2D coordinate;
@property (nonatomic, retain) NSString *title;
@property (nonatomic, retain) NSString *subtitle;

- (id)initWithLat:(float)lat lon:(float)lon;

@end
