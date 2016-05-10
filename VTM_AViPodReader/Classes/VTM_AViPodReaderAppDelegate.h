//
//  VTM_AViPodReaderAppDelegate.h
//  VTM_AViPodReader
//
//  Created by Chris Adamson on 10/16/10.
//  Copyright 2010 Subsequently and Furthermore, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@class VTM_AViPodReaderViewController;

@interface VTM_AViPodReaderAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    VTM_AViPodReaderViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet VTM_AViPodReaderViewController *viewController;

@end

