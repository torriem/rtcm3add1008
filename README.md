rtcm3add1008
============

This is a simple sketch for Arduino Uno (or similar boards) that receives
RTCMv3 messages on the built-in main Serial port, parses them enough to
recognize the message type, and transmits them back out a software serial
port on other pins.  After each 1005 message is recognized and passed
through, the sketch adds a blank 1008 message type to the output.  

This was created because the ZED-F9P cannot generate a 1008 message, so
some GPS receivers like the Trimble 372, which expect it, cannot
resolve to an RTK fix.

This little filter could be placed between the base station ZED-F9P and the
radio modem or could be placed on the rover between whatever receives the 
RTCMv3 messages and the rover ZED-F9P.  This could could be merged into a
project like https://github.com/ardusimple/simpleRTK2B to have this built
into an ESP32-based NTRIP Caster or NTRIP server.

I use it on my base station, between the ZED-F9P and the radio modem.  I
have another small computer that listens on the radio modem and transmits
the RTCMv3 stream via the internet for use with the android NTRIP Client
app.  In the future I'll combine them using code from the above-mentioned
simpleRTK2B repository.


