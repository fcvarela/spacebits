#import <UIKit/UIKit.h>

@interface SpacebitsMobileAppDelegate : NSObject <UIApplicationDelegate, UITabBarControllerDelegate> {
    UIWindow *window;
    UITabBarController *tabBarController;
	NSMutableData *responseData;
	NSDictionary *telemetry;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UITabBarController *tabBarController;
@property (nonatomic, retain) NSDictionary *telemetry;

- (void)spawnTelemetryRequest;

@end
