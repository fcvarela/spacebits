//
//  TelemetryViewController.h
//  SpacebitsMobile
//
//  Created by Filipe Varela on 10/05/26.
//  Copyright 2010 Caixa Mágica Software. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TelemetryViewController : UITableViewController {
	NSDictionary *telemetry;
}

@property (nonatomic, retain) NSDictionary *telemetry;

@end
