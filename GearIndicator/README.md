## Struct information as provided by Codemasters
This project works with the 2017 version of Codemasters F1 game. The officieal specification of the UDP packets van be found here:
http://forums.codemasters.com/discussion/53139/f1-2017-d-box-and-udp-output-specification

There is a lot of information that can be extracted from the packets, but we only use the `gear` field.

## Usage
For the hardware to work, it needs a (preferably fixed) IP address. For easy Wifi management it makes use of WifiManager. WifiManager tries to connect to a previously configured access point if it has one. If it hasn't (if this is the first time you're starting it up, or if your access point has changed) is starts its own Wifi access point with the SSID `wemosGears`. You need to connect to that access point with a phone or laptop and when connected, navigate to http://192.168.4.1 to open WifiManager. From that web interface you can configure the actual access point the decice should connect to and it should remember those
settings for any subsequent connection attempt.

It is advisable to assign a static IP address to the mac address of your Wemos, this makes configuring access in the game a one-time action.

If you have a network connection and know the ip address of the device, you can configure F1 2017's UDP telemetry and see the gears change!