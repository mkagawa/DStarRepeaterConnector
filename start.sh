#!/bin/sh
echo "starting repeaterConnector"
./repeaterconnector -callsign KK6JA -mod1 A,20011 -mod2 B,20013 -rptcmd ../OpenDV/DStarRepeater/dstarrepeaterd -rcfg /usr/local/etc/opendv/dstarrepeater -logdir /var/log -confdir /var/conf

