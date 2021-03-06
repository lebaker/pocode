//
//  poOpenGLView.m
//  poFramework4
//
//  Created by Joshua Fisher on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#import "poOpenGLView.h"
#import "AppDelegate.h"
#include "poWindow.h"

CVReturn MyDisplayLinkCallback (CVDisplayLinkRef displayLink,
								const CVTimeStamp *inNow,
								const CVTimeStamp *inOutputTime,
								CVOptionFlags flagsIn,
								CVOptionFlags *flagsOut,
								void *displayLinkContext)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	poOpenGLView *self = (poOpenGLView*)displayLinkContext;
	
	if(CVDisplayLinkIsRunning(displayLink)) {
		[self stepApplication];
	}
	
	[pool release];
	return kCVReturnSuccess;
}

@implementation poOpenGLView

@synthesize appWindow;
@synthesize fullscreen;

+(NSOpenGLPixelFormat*)defaultPixelFormat {
	NSOpenGLPixelFormatAttribute attributes[] = {
		NSOpenGLPFADoubleBuffer, YES,
		NSOpenGLPFAWindow, YES,
		NSOpenGLPFADepthSize, 32,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFAAccumSize, 64,
		NSOpenGLPFAMultisample,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 4,
		0
	};
	return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

-(id)initWithFrame:(NSRect)frm 
		   context:(NSOpenGLContext*)cxt
			window:(poWindow*)win
{
	self = [super initWithFrame:frm pixelFormat:[poOpenGLView defaultPixelFormat]];
	if(self) {
		self.openGLContext = cxt;
		
		[self.openGLContext makeCurrentContext];
		self.appWindow = win;
		
		display_link = nil;
		self.fullscreen = NO;
	}
	return self;
}

-(void)dealloc {
	[self stopAnimating];
	
	delete self.appWindow;
	self.appWindow = nil;

	[super dealloc];
}

-(BOOL)acceptsFirstResponder {
	return YES;
}

-(BOOL)isFlipped {
	return YES;
}

-(void)viewWillMoveToWindow {
	// lets go ahead and draw one so the framebuffer is ready when we're visible
	[self stepApplication];
}

-(void)viewDidMoveToWindow {
	// make sure we cancelled any notifications we were working with
	[[NSNotificationCenter defaultCenter] removeObserver:self name:NSWindowDidResizeNotification object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:NSWindowDidChangeScreenNotification object:nil];
	
	// we were either added or removed from a window
	if(self.window) {
		void (^resizeBlock)(NSNotification*) = ^(NSNotification*){
			if(self.appWindow && self.window) {
				NSRect rect = [self.window contentRectForFrameRect:self.window.frame];
				rect.origin = self.window.frame.origin;
				self.appWindow->resized(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
			}
		};
		
		resizeBlock(nil);
		[[NSNotificationCenter defaultCenter] addObserverForName:NSWindowDidResizeNotification 
														  object:self.window
														   queue:nil
													  usingBlock:resizeBlock];

		[[NSNotificationCenter defaultCenter] addObserverForName:NSWindowDidChangeScreenNotification 
														  object:self.window
														   queue:nil
													  usingBlock:^(NSNotification*) {
														  [self stopAnimating];
														  [self startAnimating];
													  }];
		
		[self startAnimating];
	}
	else {
		[self stopAnimating];
	}
}

-(void)viewWillStartLiveResize {
	[self stopAnimating];
	[super viewWillStartLiveResize];
}

-(BOOL)isAnimating {
	return display_link != nil;
}

-(void)startAnimating {
	if(!display_link) {
		CGDirectDisplayID displayID = (CGDirectDisplayID)[[[[self.window screen] deviceDescription] objectForKey:@"NSScreenNumber"] intValue];
		CVDisplayLinkCreateWithCGDisplay(displayID, &display_link);
		CVDisplayLinkSetOutputCallback(display_link, MyDisplayLinkCallback, self);
		CVDisplayLinkStart(display_link);
	}
}

-(void)stopAnimating {
	if(display_link) {
		CVDisplayLinkStop(display_link);
		CVDisplayLinkRelease(display_link);
		display_link = nil;
	}
}

-(void)stepApplication {
	[self.openGLContext makeCurrentContext];
	self.appWindow->makeCurrent();
	self.appWindow->update();
	self.appWindow->draw();
	[self.openGLContext flushBuffer];
}

-(void)keyDown:(NSEvent*)event {
	if(event.characters.length)
		self.appWindow->keyDown([event.characters characterAtIndex:0], event.keyCode, (int)event.modifierFlags);
}

-(void)keyUp:(NSEvent*)event {
	if(event.characters.length)
		self.appWindow->keyUp([event.characters characterAtIndex:0], event.keyCode, (int)event.modifierFlags);
}

-(void)mouseDown:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseDown(point.x, point.y, 0);
}

-(void)mouseUp:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseUp(point.x, point.y, 0);
}

-(void)mouseMoved:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseMove(point.x, point.y, 0);
}

-(void)mouseDragged:(NSEvent*)event {
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
	self.appWindow->mouseDrag(point.x, point.y, 0);
}

@end
