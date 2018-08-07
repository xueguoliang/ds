#!/bin/sh

#ps aux | grep DS | awk '{print $2}' | xargs kill -kill

if [ -f "~/Library/LaunchAgents/cn.xueguoliang.dscap.plist" ]; then
    launchctl unload ~/Library/LaunchAgents/cn.xueguoliang.dscap.plist
fi
if [ -f "~/Library/LaunchAgents/cn.xueguoliang.dssend.plist" ]; then
    launchctl unload ~/Library/LaunchAgents/cn.xueguoliang.dssend.plist
fi

#ps aux | grep DSCap | awk '{print $2}' | xargs kill -kill
#ps aux | grep DSSend | awk '{print $2}' | xargs kill -kill


rm -rf ~/Library/LaunchAgents/cn.xueguoliang.dscap.plist
rm -rf ~/Library/LaunchAgents/cn.xueguoliang.dssend.plist
rm -rf ~/Library/DS 
rm -rf ~/Applications/DS.app
rm -rf ~/Applications/DSView.app

rm -rf /Applications/DS.app
rm -rf /Applications/DSView.app


