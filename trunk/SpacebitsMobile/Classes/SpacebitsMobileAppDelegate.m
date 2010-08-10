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
	NSLog(@"Current rate is: %d", rate);
	switch (rate) {
	case 0: rate = 5; break;
	case 1: rate = 10; break;
	case 2: rate = 14; break;
	}
	[NSTimer scheduledTimerWithTimeInterval:rate target:self selector:@selector(onTimer) userInfo:nil repeats:YES];
	
	return YES;
}

- (void)onTimer
{
	[self spawnTelemetryRequest];
}

- (void)spawnTelemetryRequest
{
	NSInteger balloonId = [[NSUserDefaults standardUserDefaults] integerForKey:@"balloonId"];
	if (balloonId == 0)
		balloonId = 1;
	NSString *urlString = [NSString stringWithFormat:@"http://spacebits.eu/api/get/%d", balloonId];
	NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:urlString]];	
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
	
	// selected balloon
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

