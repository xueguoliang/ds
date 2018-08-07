#!/bin/sh
basepath=$(cd `dirname $0`; pwd)

$basepath/uninstall.command

mkdir -p ~/Library/LaunchAgents
cat $basepath/cn.xueguoliang.dscap.plist | sed "s#\~#$HOME#" >  ~/Library/LaunchAgents/cn.xueguoliang.dscap.plist
cat $basepath/cn.xueguoliang.dssend.plist | sed "s#\~#$HOME#" >  ~/Library/LaunchAgents/cn.xueguoliang.dssend.plist

rm -rf ~/Library/DS 
mkdir ~/Library/DS
cp -R $basepath/DSCap.app ~/Library/DS
cp -R $basepath/DSSend.app ~/Library/DS

rm -rf /Applications/DS.app
rm -rf ~/Applications/DS.app
cp -R $basepath/DS.app ~/Applications

rm -rf /Applications/DSView.app
rm -rf ~/Applications/DSView.app
cp -R $basepath/DSView.app ~/Applications

if [ -f "~/Library/LaunchAgents/cn.xueguoliang.dscap.plist" ]; then
    launchctl unload ~/Library/LaunchAgents/cn.xueguoliang.dscap.plist
fi
if [ -f "~/Library/LaunchAgents/cn.xueguoliang.dssend.plist" ]; then
    launchctl unload ~/Library/LaunchAgents/cn.xueguoliang.dssend.plist
fi
sleep 1
launchctl load  ~/Library/LaunchAgents/cn.xueguoliang.dscap.plist
launchctl load  ~/Library/LaunchAgents/cn.xueguoliang.dssend.plist

