#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#include "../telemetry-daemon/telemetry.h"

@interface spacebits_groundstationAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
	IBOutlet id mapView;
	sensor_data_t telemetry;
}

@property (assign) IBOutlet NSWindow *window;

- (IBAction)ctrOnAltair:(id)sender;
- (void)moveToLatitude:(float)latitude longitude:(float)longitude;

@end
