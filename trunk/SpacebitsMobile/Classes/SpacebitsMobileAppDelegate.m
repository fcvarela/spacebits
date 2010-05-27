#import "SpacebitsMobileAppDelegate.h"

@implementation SpacebitsMobileAppDelegate

@synthesize window;
@synthesize tabBarController;
@synthesize telemetry;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Add the tab bar controller's current view as a subview of the window
    [window addSubview:tabBarController.view];
    [window makeKeyAndVisible];
	
	responseData = [[NSMutableData data] retain];
	NSInteger rate = [[NSUserDefaults standardUserDefaults] integerForKey:@"telemetryRate"];
	if (rate == 0) rate = 2;
	[NSTimer scheduledTimerWithTimeInterval:rate target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
	
	return YES;
}

- (void)onTimer
{
	[self spawnTelemetryRequest];
}

- (void)spawnTelemetryRequest
{
	NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://spacebits.eu/api/get"]];	
	[[NSURLConnection alloc] initWithRequest:request delegate:self];
	
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
	[responseData setLength:0];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
	[responseData appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
	
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
	[connection release];
	
	NSString *responseString = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
	NSDictionary *d_telemetry = [responseString JSONValue];
	
	self.telemetry = d_telemetry;
}

- (void)dealloc {
    [tabBarController release];
    [window release];
    [super dealloc];
}

@end

