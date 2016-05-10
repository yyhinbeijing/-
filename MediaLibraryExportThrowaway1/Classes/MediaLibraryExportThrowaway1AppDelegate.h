//
//  MediaLibraryExportThrowaway1AppDelegate.h
//  MediaLibraryExportThrowaway1
//
//  Created by Chris Adamson on 7/16/10.
//  http://www.subfurther.com/
//  Released into the public domain, 7/19/10
//

#import <UIKit/UIKit.h>

@class MediaLibraryExportThrowaway1ViewController;

@interface MediaLibraryExportThrowaway1AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    MediaLibraryExportThrowaway1ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MediaLibraryExportThrowaway1ViewController *viewController;

@end

