//
//  AltairAnnotation.m
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/19.
//  Copyright 2010 Caixa Mágica Software. All rights reserved.
//

#import "AltairAnnotation.h"


@implementation AltairAnnotation

@synthesize coordinate;

- (id)initWithLat:(float)lat lon:(float)lon
{
	self = [super init];
	
	coordinate.latitude = lat;
	coordinate.longitude = lon;
	
	return self;
}

@end
