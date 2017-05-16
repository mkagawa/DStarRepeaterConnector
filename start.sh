#!/bin/sh
echo "starting repeaterConnector"
./repeaterconnector -callsign KK6JA -mod1 A,20013 -mod2 E,20014 -rptcmd ../OpenDV/DStarRepeater/dstarrepeaterd -logdir:./logs -confdir ./conf -startrptr -tx

