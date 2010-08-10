#import <UIKit/UIKit.h>

@interface SettingsViewController : UITableViewController {
	UISegmentedControl *mapSegmentedControl;
	UISegmentedControl *telemetrySegmentedControl;
	UISegmentedControl *balloonIdSegmentedControl;
}

@property (nonatomic, retain) UISegmentedControl *mapSegmentedControl;
@property (nonatomic, retain) UISegmentedControl *telemetrySegmentedControl;
@property (nonatomic, retain) UISegmentedControl *balloonIdSegmentedControl;

@end
