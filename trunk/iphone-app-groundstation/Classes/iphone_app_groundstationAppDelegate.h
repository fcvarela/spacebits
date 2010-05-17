//
//  iphone_app_groundstationAppDelegate.h
//  iphone-app-groundstation
//
//  Created by Filipe Varela on 10/05/17.
//  Copyright Caixa Mágica Software 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class iphone_app_groundstationViewController;

@interface iphone_app_groundstationAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    iphone_app_groundstationViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet iphone_app_groundstationViewController *viewController;

@end

