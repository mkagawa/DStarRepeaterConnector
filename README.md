# DStarRepeaterConnector

This program is supposed to use to connect two DstarRepeater instances.

Configure your environment with special attention, not to create any packet loop.
It may causing a messing up of whole D-Star network, not only your local network.

## Disclaimer

  Copyright (C) 2017 by Masahito Kagawa

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 

## How this program works

this program runs only linux environment. The program simulates DVAP dongle,
by creating virtual serial port.
This program automatically start two dstarrepeater with appropriate configuration.
the progran can be terminated by pressing Ctrl-C or send SIGTERM signal. dstarrepeater
processes will be terminated as well.

Note that Gateway / Repeater callsign must be different from user's callsign.
Any D-Star packets from same callsign as the repeater callsign won't be forwarded
to the other repeater.

## How to setup
Here is the steps to setup
1. Prepare G4KLX's ircddbgateway - dstarrepeater setup in single linux box.
2. Configure your ircddbgateway appropriately. Setup two modules (say A and B) for this connector.
   - Type = Homebrew
   - Address = 127.0.0.1
   - Port = 20011 for A, 20012 for B (use whatever appropriate port number for this)
   - Reflector = empty / Startup = No / Reconnect = Never (you can eventually change this)
3. Create your own start.sh file by copying original start.sh. Change callsign field and port number
with modile id with -mod1 and -mod2 parameter which are matching with ircddbgateway's configuration.
specify dstarrepater binary path (full path is recommended, ex. -rptcmd /usr/local/sbin/dstarrepeater)
-logdir, -confdir must be writable by user which runs this program. 
If you don't specify -startrptr (start dstarrepeater), the program won't start dstarrepeater. You can manually
start it, by checking repeaterconnector.log file.
4. Here is an example of command line.
<pre>
./repeaterconnector -callsign KK6JA -mod1 A,20013 -mod2 E,20014 -rptcmd /usr/local/sbin/dstarrepeaterd
   -logdir ./logs -confdir ./conf --verbose -startrptr -tx
</pre>

## Linking to the reflectors

to link the reflectors from each dstarrepeater instance, recommend to use remotecontrold command
or other remote control programs. Here is the command line example of remotecontrold.

<pre>
must execute from the same machine as ircddbgateway

Link module A to REF008-D
$ /usr/local/bin/remotecontrold "KK6JA  A" link never "REF008 D"

Unlink module A
$ /usr/local/bin/remotecontrold "KK6JA  A" unlink
</pre>

## Other configuration parameters
<pre>
Usage: repeaterconnector [-v] -callsign &lt;str> [-gwport &lt;num>] -mod1 &lt;str> -mod2 &lt;str>
      -rptcmd &lt;str> [-confdir &lt;str>] [-logdir &lt;str>] [-startrptr] [-tx] [-dump] [-h] [--verbose]
  -v                    show version
  -callsign:&lt;str>       gw and repeater base callsign without suffix
  -gwport:&lt;num>         gw port number (default:20010)
  -mod1:&lt;str>           dstarrepeater 1 module letter [A-E] and port num. (ex: -mod1:A,20011)
  -mod2:&lt;str>           dstarrepeater 2 module letter [A-E] and port num. (ex: -mod2:A,20011)
  -rptcmd:&lt;str>         full path of dstarrepeater executable
  -confdir:&lt;str>        config file directory (default: current dir)
  -logdir:&lt;str>         log directory (default: current dir)
  -startrptr            start dstarrepeater
  -tx                   enables forewarding packet in each repeaters
  -dump                 dump packets in log file with --verbose
  -h, --help            show this help message
  --verbose             generate verbose log messages
</pre>

