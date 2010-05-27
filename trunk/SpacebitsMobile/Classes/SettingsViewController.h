#import <UIKit/UIKit.h>

@interface SettingsViewController : UITableViewController {
	UISegmentedControl *mapSegmentedControl;
	UISegmentedControl *telemetrySegmentedControl;
}

@property (nonatomic, retain) UISegmentedControl *mapSegmentedControl;
@property (nonatomic, retain) UISegmentedControl *telemetrySegmentedControl;

@end
