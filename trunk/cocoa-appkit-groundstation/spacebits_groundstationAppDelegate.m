#import "spacebits_groundstationAppDelegate.h"

@implementation spacebits_groundstationAppDelegate
@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
}

- (NSNumber *)latitude {
	return [NSNumber numberWithFloat: telemetry.latitude];
}

- (void)awakeFromNib
{
	NSBundle *bundle = [NSBundle mainBundle];
	NSString *path = [bundle bundlePath];
	NSString *fullPath = [NSBundle pathForResource:@"mapbase" ofType:@"html" inDirectory:path];

	[[mapView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:fullPath]]];
	[[[mapView mainFrame] frameView] setAllowsScrolling:NO];
	[mapView setNeedsDisplay:YES];
	
	telemetry.latitude = 39.0;
	telemetry.longitude = -9.0;
}

- (IBAction)ctrOnAltair:(id)sender
{
	[self moveToLatitude:telemetry.latitude longitude:telemetry.longitude];
}

- (void)moveToLatitude:(float)latitude longitude:(float)longitude
{
	id map = [mapView windowScriptObject];
	NSString *strLat = [NSString stringWithFormat:@"%f", latitude];
	NSString *strLon = [NSString stringWithFormat:@"%f", longitude];
	NSString *jsCmd1 = [NSString stringWithFormat: @"map.setMapCenter(new OpenLayers.LonLat(%@, %@));", strLon, strLat];
	NSString *jsCmd2 = [NSString stringWithFormat: @"altair.setLonLat(new OpenLayers.LonLat(%@, %@));", strLon, strLat];
	[map evaluateWebScript:jsCmd1];
	[map evaluateWebScript:jsCmd2];
}

@end
