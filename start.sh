#!/bin/sh
echo "starting repeaterConnector"
./repeaterconnector -callsign KK6JA -mod1 A,20011 -mod2 E,20015 -rptcmd ../OpenDV/DStarRepeater/dstarrepeaterd -rcfg /usr/local/etc/opendv/dstarrepeater -logdir:./log -confdir ./conf --verbose

