//
//  VTM_AViPodReaderViewController.m
//  VTM_AViPodReader
//
//  Created by Chris Adamson on 10/16/10.
//  Copyright 2010 Subsequently and Furthermore, Inc. All rights reserved.
//

#import "VTM_AViPodReaderViewController.h"
#import <AudioToolbox/AudioToolbox.h> // for the core audio constants


#define EXPORT_NAME @"exported.caf"

@implementation VTM_AViPodReaderViewController

@synthesize songLabel;
@synthesize artistLabel;
@synthesize sizeLabel;
@synthesize coverArtView;
@synthesize conversionProgress;


#pragma mark init/dealloc
- (void)dealloc {
    [super dealloc];
}

#pragma mark vc lifecycle

-(void) viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];
}

#pragma mark event handlers

-(IBAction) chooseSongTapped: (id) sender {
	MPMediaPickerController *pickerController =	[[MPMediaPickerController alloc]
												 initWithMediaTypes: MPMediaTypeMusic];
	pickerController.prompt = @"Choose song to export";
	pickerController.allowsPickingMultipleItems = NO;
	pickerController.delegate = self;
	[self presentModalViewController:pickerController animated:YES];
	[pickerController release];
	
}

-(IBAction) convertTapped: (id) sender {
	// set up an AVAssetReader to read from the iPod Library
	NSURL *assetURL = [song valueForProperty:MPMediaItemPropertyAssetURL];
	AVURLAsset *songAsset = [AVURLAsset URLAssetWithURL:assetURL options:nil];

	NSError *assetError = nil;
	AVAssetReader *assetReader = [[AVAssetReader assetReaderWithAsset:songAsset
															   error:&assetError]
								  retain];
	if (assetError) {
		NSLog (@"error: %@", assetError);
		return;
	}
	
	AVAssetReaderOutput *assetReaderOutput = [[AVAssetReaderAudioMixOutput 
											  assetReaderAudioMixOutputWithAudioTracks:songAsset.tracks
																		audioSettings: nil]
											  retain];
	if (! [assetReader canAddOutput: assetReaderOutput]) {
		NSLog (@"can't add reader output... die!");
		return;
	}
	NSLog (@"assetReaderOutput.mediaType = %@", assetReaderOutput.mediaType);
	[assetReader addOutput: assetReaderOutput];
	
	NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectoryPath = [dirs objectAtIndex:0];
	NSString *exportPath = [[documentsDirectoryPath stringByAppendingPathComponent:EXPORT_NAME] retain];
	if ([[NSFileManager defaultManager] fileExistsAtPath:exportPath]) {
		[[NSFileManager defaultManager] removeItemAtPath:exportPath error:nil];
	}
	NSURL *exportURL = [NSURL fileURLWithPath:exportPath];
	AVAssetWriter *assetWriter = [[AVAssetWriter assetWriterWithURL:exportURL
														  fileType:AVFileTypeCoreAudioFormat
															 error:&assetError]
								  retain];
	if (assetError) {
		NSLog (@"error: %@", assetError);
		return;
	}
	AudioChannelLayout channelLayout;
	memset(&channelLayout, 0, sizeof(AudioChannelLayout));
	channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
	NSDictionary *outputSettings = [NSDictionary dictionaryWithObjectsAndKeys:
									[NSNumber numberWithInt:kAudioFormatLinearPCM], AVFormatIDKey, 
									[NSNumber numberWithFloat:44100.0], AVSampleRateKey,
									[NSNumber numberWithInt:2], AVNumberOfChannelsKey,
									[NSData dataWithBytes:&channelLayout length:sizeof(AudioChannelLayout)], AVChannelLayoutKey,
									[NSNumber numberWithInt:16], AVLinearPCMBitDepthKey,
									[NSNumber numberWithBool:NO], AVLinearPCMIsNonInterleaved,
									[NSNumber numberWithBool:NO],AVLinearPCMIsFloatKey,
									[NSNumber numberWithBool:NO], AVLinearPCMIsBigEndianKey,
									nil];
	AVAssetWriterInput *assetWriterInput = [[AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeAudio
																			  outputSettings:outputSettings]
											retain];
	if ([assetWriter canAddInput:assetWriterInput]) {
		[assetWriter addInput:assetWriterInput];
	} else {
		NSLog (@"can't add asset writer input... die!");
		return;
	}
	
	assetWriterInput.expectsMediaDataInRealTime = NO;

	[assetWriter startWriting];
	[assetReader startReading];

	AVAssetTrack *soundTrack = [songAsset.tracks objectAtIndex:0];
	CMTime startTime = CMTimeMake (0, soundTrack.naturalTimeScale);
	[assetWriter startSessionAtSourceTime: startTime];
	
	__block UInt64 convertedByteCount = 0;
	
	dispatch_queue_t mediaInputQueue = dispatch_queue_create("mediaInputQueue", NULL);
	[assetWriterInput requestMediaDataWhenReadyOnQueue:mediaInputQueue 
											usingBlock: ^ 
	 {
		 // NSLog (@"top of block");
		 while (assetWriterInput.readyForMoreMediaData) {
			CMSampleBufferRef nextBuffer = [assetReaderOutput copyNextSampleBuffer];
			if (!nextBuffer) {
				// done?
				[assetWriterInput markAsFinished];
				[assetWriter finishWriting];
				[assetReader cancelReading];
				NSDictionary *outputFileAttributes = [[NSFileManager defaultManager]
													  attributesOfItemAtPath:exportPath
													  error:nil];
				NSLog (@"done. file size is %ld",
					    [outputFileAttributes fileSize]);
				NSNumber *doneFileSize = [NSNumber numberWithLong:[outputFileAttributes fileSize]];
				[self performSelectorOnMainThread:@selector(updateCompletedSizeLabel:)
									   withObject:doneFileSize
									waitUntilDone:NO];
				// release a lot of stuff
				[assetReader release];
				[assetReaderOutput release];
				[assetWriter release];
				[assetWriterInput release];
				[exportPath release];
				break;
			} else {
				// append buffer
				[assetWriterInput appendSampleBuffer: nextBuffer];
//				NSLog (@"appended a buffer (%d bytes)", 
//					   CMSampleBufferGetTotalSampleSize (nextBuffer));
				convertedByteCount += CMSampleBufferGetTotalSampleSize (nextBuffer);
				// oops, no
				// sizeLabel.text = [NSString stringWithFormat: @"%ld bytes converted", convertedByteCount];

				NSNumber *convertedByteCountNumber = [NSNumber numberWithLong:convertedByteCount];
				[self performSelectorOnMainThread:@selector(updateSizeLabel:)
									   withObject:convertedByteCountNumber
									waitUntilDone:NO];
			}
		}

	 }];
	NSLog (@"bottom of convertTapped:");
}

-(void) updateSizeLabel: (NSNumber*) convertedByteCountNumber {
	UInt64 convertedByteCount = [convertedByteCountNumber longValue];
	sizeLabel.text = [NSString stringWithFormat: @"%ld bytes converted", convertedByteCount];
}

-(void) updateCompletedSizeLabel: (NSNumber*) convertedByteCountNumber {
	UInt64 convertedByteCount = [convertedByteCountNumber longValue];
	sizeLabel.text = [NSString stringWithFormat: @"done. file size is %ld", convertedByteCount];
}


#pragma mark MPMediaPickerControllerDelegate
- (void)mediaPicker: (MPMediaPickerController *)mediaPicker
  didPickMediaItems:(MPMediaItemCollection *)mediaItemCollection {
	[self dismissModalViewControllerAnimated:YES];
	if ([mediaItemCollection count] < 1) {
		return;
	}
	[song release];
	song = [[[mediaItemCollection items] objectAtIndex:0] retain];
	songLabel.hidden = NO;
	artistLabel.hidden = NO;
	coverArtView.hidden = NO;
	songLabel.text = [song valueForProperty:MPMediaItemPropertyTitle];
	artistLabel.text = [song valueForProperty:MPMediaItemPropertyArtist];
	coverArtView.image = [[song valueForProperty:MPMediaItemPropertyArtwork]
						  imageWithSize: coverArtView.bounds.size];
}

- (void)mediaPickerDidCancel:(MPMediaPickerController *)mediaPicker {
	[self dismissModalViewControllerAnimated:YES];
}


@end
