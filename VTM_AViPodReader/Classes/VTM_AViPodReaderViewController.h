//
//  VTM_AViPodReaderViewController.h
//  VTM_AViPodReader
//
//  Created by Chris Adamson on 10/16/10.
//  Copyright 2010 Subsequently and Furthermore, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <MediaPlayer/MediaPlayer.h>

@interface VTM_AViPodReaderViewController : UIViewController <MPMediaPickerControllerDelegate>{
	MPMediaItem *song;

	UILabel *songLabel;
	UILabel *artistLabel;
	UILabel *sizeLabel;
	UIImageView *coverArtView;
	UIProgressView *conversionProgress;
	
}

@property (nonatomic, retain) IBOutlet UILabel *songLabel;
@property (nonatomic, retain) IBOutlet UILabel *artistLabel;
@property (nonatomic, retain) IBOutlet UILabel *sizeLabel;
@property (nonatomic, retain) IBOutlet UIImageView *coverArtView;
@property (nonatomic, retain) IBOutlet UIProgressView *conversionProgress;

-(IBAction) chooseSongTapped: (id) sender;
-(IBAction) convertTapped: (id) sender;

@end

