//
//  main.m
//  WLAN-UDP
//
//  Created by Rinc Liu on 18/11/2017.
//  Copyright © 2017 RINCLIU. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WLAN : NSObject

@property(nonatomic,copy) NSString* SSIDName;

@property(nonatomic,copy) NSString* SSIDCode;

@property(nonatomic,copy) NSString* gatewayAddress;

@property(nonatomic,copy) NSString* broadcastAddress;

@property(nonatomic,copy) NSString* localAddress;

@property(nonatomic,copy) NSString* subnetMask;

@end

extern NSString* const kNotificationNetworkChanged;

@interface NetworkUtil : NSObject

+(WLAN*)currentWLAN;

+(void)addNetworkChangeObserver:(id)observer selector:(SEL)selector;

@end
