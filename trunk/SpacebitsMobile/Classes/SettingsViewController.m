#import "SettingsViewController.h"

@implementation SettingsViewController

@synthesize mapSegmentedControl;
@synthesize telemetrySegmentedControl;

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 2;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	NSString *outStr = @"Telemetry Rate";
	
	if (section == 0)
		outStr = @"Map Settings";
	
	return outStr;
	
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"Cell";
    
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil)
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
	
	if (mapSegmentedControl == nil) {
		mapSegmentedControl = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObjects:@"Map", @"Satellite", @"Hybrid", nil]];
		mapSegmentedControl.frame = CGRectMake(-1, -1, 301, 47);
		[mapSegmentedControl addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
		mapSegmentedControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:@"mapType"];
	}
	
	if (telemetrySegmentedControl == nil) {
		telemetrySegmentedControl = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObjects:@"5 Sec", @"10 Sec", @"15 Sec", nil]];
		telemetrySegmentedControl.frame = CGRectMake(-1, -1, 301, 47);
		[telemetrySegmentedControl addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
		telemetrySegmentedControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:@"telemetryRate"];
	}
	
	if (indexPath.section == 0)
		[cell.contentView addSubview:mapSegmentedControl];
	else
		[cell.contentView addSubview:telemetrySegmentedControl];

    return cell;
}

- (void)segmentAction:(UISegmentedControl *)sender
{
	if (sender == mapSegmentedControl)
		[[NSUserDefaults standardUserDefaults] setInteger:[sender selectedSegmentIndex] forKey:@"mapType"];
	
	if (sender == telemetrySegmentedControl) {
		[[NSUserDefaults standardUserDefaults] setInteger:[sender selectedSegmentIndex] forKey:@"telemetryRate"];
	}
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
}


- (void)dealloc {
    [super dealloc];
}


@end

