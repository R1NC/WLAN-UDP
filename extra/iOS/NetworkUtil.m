//
//  main.m
//  WLAN-UDP
//
//  Created by Rinc Liu on 18/11/2017.
//  Copyright © 2017 RINCLIU. All rights reserved.
//

#import "NetworkUtil.h"
#import <SystemConfiguration/CaptiveNetwork.h>
#import "net_util.h"

#define str2ref(str) (__bridge CFStringRef)str
#define ref2str(ref) (__bridge NSString*)ref
#define char2str(charP) [NSString stringWithCString:charP encoding:NSUTF8StringEncoding]

NSString* const kNotificationNetworkChanged = @"NotificationNetworkChanged";
static const NSString* NotifyNetChange = @"com.apple.system.config.network_change";

static void onNotifyCallback(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
    NSString* notifyName = ref2str(name);
    if ([NotifyNetChange isEqualToString:notifyName]) {
        [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationNetworkChanged object:nil userInfo:nil];
    }
}

@implementation WLAN

@end

@implementation NetworkUtil

+(WLAN*)currentWLAN {
    WLAN* wlan = [WLAN new];
    
    NSArray *ifs = (__bridge_transfer NSArray*)CNCopySupportedInterfaces();
    if (ifs) {
        NSDictionary *ssid = nil;
        for (NSString *_if in ifs) {
            ssid = (__bridge_transfer NSDictionary*)CNCopyCurrentNetworkInfo(str2ref(_if));
            if (ssid && [ssid count]) {
                wlan.SSIDCode = ssid[ref2str(kCNNetworkInfoKeyBSSID)];
                wlan.SSIDName = ssid[ref2str(kCNNetworkInfoKeySSID)];
                break;
            }
        }
    }
    
    struct wlan_info wi = get_wlan_info();
    if (wi.local_ip) wlan.localAddress = char2str(wi.local_ip);
    if (wi.gateway_address) wlan.gatewayAddress = char2str(wi.gateway_address);
    if (wi.broadcast_address) wlan.broadcastAddress = char2str(wi.broadcast_address);
    if (wi.subnet_mask) wlan.subnetMask = char2str(wi.subnet_mask);
    
    return wlan;
}

+(void)addNetworkChangeObserver:(id)observer selector:(SEL)selector {
    CFNotificationCenterAddObserver(CFNotificationCenterGetDarwinNotifyCenter(), NULL, onNotifyCallback, str2ref(NotifyNetChange), NULL, CFNotificationSuspensionBehaviorDeliverImmediately);
    [[NSNotificationCenter defaultCenter]addObserver:observer selector:selector name:kNotificationNetworkChanged object:nil];
}

@end
