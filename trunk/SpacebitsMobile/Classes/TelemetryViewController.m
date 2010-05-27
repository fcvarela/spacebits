#import "SpacebitsMobileAppDelegate.h"
#import "TelemetryViewController.h"

@implementation TelemetryViewController
@synthesize telemetry;

- (void)viewDidLoad {
	[super viewDidLoad];
	[[[UIApplication sharedApplication] delegate] addObserver:self forKeyPath:@"telemetry" options:NSKeyValueObservingOptionNew context:nil];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 3;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	NSString *secName;
	
	if (section == 0)
		secName = @"Flight Stats";
	
	if (section == 1)
		secName = @"Position";
	
	if (section == 2)
		secName = @"Sensor Array";
	
	return secName;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	NSInteger rows = 0;
	
	switch (section) {
		case 0: rows = 2; break;
		case 1: rows = 4; break;
		case 2: rows = [telemetry count]; break;
	}

	return rows;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
	if ([keyPath isEqualToString:@"telemetry"]) {
		self.telemetry = [object telemetry];
		[self.tableView performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:NO];
	}
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }
    
	/*alt = "9000.0";
    bear = "1024.0";
    change = 1274805014;
    "dust_density" = "1024.0";
    elapsed = "00:00:01 14:59:44 17:30:14";
    humidity = "1024.0";
    "imu_ax" = "1024.0";
    "imu_ay" = "1024.0";
    "imu_az" = "1024.0";
    "imu_gx" = "1024.0";
    "imu_gy" = "1024.0";
    lat = "37.770927";
    lon = "-8.0829";
    "power_current" = "1024.0";
    "power_voltage" = "1024.0";
    pressure = "2048.0";
    temperature = "2048.0";
	*/
	switch (indexPath.section) {
		case 0:
			switch (indexPath.row) {
				case 0:
					cell.textLabel.text = @"Change Count";
					cell.detailTextLabel.text = [telemetry valueForKey:@"change"];
					break;
				case 1: 
					cell.textLabel.text = @"Elapsed Time";
					cell.detailTextLabel.text = [telemetry valueForKey:@"elapsed"];
					break;
			}
			break;
		case 1:
			switch (indexPath.row) {
				case 0:
					cell.textLabel.text = @"Latitude";
					cell.detailTextLabel.text = [telemetry valueForKey:@"lat"];
					break;
				case 1:
					cell.textLabel.text = @"Longitude";
					cell.detailTextLabel.text = [telemetry valueForKey:@"lon"];
					break;
				case 2:
					cell.textLabel.text = @"Altitude";
					cell.detailTextLabel.text = [telemetry valueForKey:@"alt"];
					break;
				case 3: 
					cell.textLabel.text = @"Bearing"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"bear"];
					break;
			}
			break;
		case 2:
		switch (indexPath.row) {
				case 0: 
					cell.textLabel.text = @"Current"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"power_current"];
					break;
				case 1: 
					cell.textLabel.text = @"Voltage"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"power_voltage"];
					break;
				case 2: 
					cell.textLabel.text = @"Pressure"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"pressure"];
					break;
				case 3: 
					cell.textLabel.text = @"Temperature"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"temperature"];
					break;
				case 4: 
					cell.textLabel.text = @"Humidity"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"humidity"];
					break;
				case 5: 
					cell.textLabel.text = @"Dust Density"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"dust_density"];
					break;
				case 6: 
					cell.textLabel.text = @"IMU Accel X"; 
					cell.detailTextLabel.text = [telemetry valueForKey:@"imu_ax"];
					break;
				case 7: 
					cell.textLabel.text = @"IMU Accel Y";
					cell.detailTextLabel.text = [telemetry valueForKey:@"imu_ay"];
					break;
				case 8: 
					cell.textLabel.text = @"IMU Accel Z";
					cell.detailTextLabel.text = [telemetry valueForKey:@"imu_az"];
					break;
				case 9: 
					cell.textLabel.text = @"IMU Rate X";
					cell.detailTextLabel.text = [telemetry valueForKey:@"imu_gx"];
					break;
				case 10: 
					cell.textLabel.text = @"IMU Rate Y";
					cell.detailTextLabel.text = [telemetry valueForKey:@"imu_gy"];
					break;
			}
			break;
	}
    return cell;
}


- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    return NO;
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end

