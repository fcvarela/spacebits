//
//  spacebits_groundstationAppDelegate.h
//  spacebits-groundstation
//
//  Created by Filipe Varela on 10/02/24.
//  Copyright 2010 Caixa Mágica Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#include "telemetry/telemetry.h"

@interface spacebits_groundstationAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
	IBOutlet id mapView;
	IBOutlet NSView *vgaCamView;
	IBOutlet NSProgressIndicator *vgaCamProgressBar;
	spacebits_telemetry_t telemetry;
	int telemetry_pipe, command_pipe;
}

@property (assign) IBOutlet NSWindow *window;

- (IBAction)ctrOnAltair:(id)sender;
- (IBAction)takeVGAPicture:(id)sender;
- (void)moveToLatitude:(float)latitude longitude:(float)longitude;

@end
