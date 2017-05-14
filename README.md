# DStarRepeaterConnector

This program is supposed to use to connect two DstarRepeater instances.

Configure your environment with special attention, not to create any packet loop. 
It may causing a messing up of whole D-Star network, not only your local network.

## how this program works

this program runs only linux environment. The program simulates DVAP dongle, 
by creating virtual serial port. 
This program automatically start two dstarrepeater with appropriate configuration. 
the progran can be terminated by pressing Ctrl-C or send SIGTERM signal. dstarrepeater
processes will be terminated as well.

## how to setup
Here is the steps to setup
1. Prepare G4KLX's ircddbgateway - dstarrepeater setup in single linux box.
2. Configure your ircddbgateway appropriately. Setup two modules (say A and B) for this connector. 
  ### Type = Homebrew
  ### Address = 127.0.0.1
  ### Port = 20011 for A, 20012 for B (use whatever appropriate port number for this)
  ### Reflector = empty / Startup = No / Reconnect = Never (you can eventually change this)
3. create your own start.sh file by copying original start.sh. Change callsign field and port number
with modile id with -mod1 and -mod2 parameter which are matching with ircddbgateway's configuration.

## other configuration parameters
Usage: repeaterconnector [-v] -callsign <str> [-gwport <num>] -mod1 <str> -mod2 <str> -rptcmd <str> [-confdir <str>] [-logdir <str>] [-startrptr] [-tx] [-dump] [-h] [--verbose]
  -v             	show version
  -callsign:<str>	gw and repeater base callsign without suffix
  -gwport:<num>  	gw port number (default:20010)
  -mod1:<str>    	dstarrepeater 1 module letter [A-E] and port num. (ex: -mod1:A,20011)
  -mod2:<str>    	dstarrepeater 2 module letter [A-E] and port num. (ex: -mod2:A,20011)
  -rptcmd:<str>  	full path of dstarrepeater executable
  -confdir:<str> 	config file directory (default: current dir)
  -logdir:<str>  	log directory (default: current dir)
  -startrptr     	start dstarrepeater
  -tx            	enables forewarding packet in each repeaters
  -dump          	dump packets in log file with --verbose
  -h, --help     	show this help message
  --verbose      	generate verbose log messages

