#This is a simple libary  (MSRP stack based on PJLIB)
<b>This is design for MSRP 4975 and MSRP 4976</b>
<br>

LMSRP is a free and open source stack written in C language implementing the Message Session Relay Protocol, it supports RFC4975 and RFC 4976, can be used for file transfer or rich message communication as data plane, always combined with PJSIP as signalling part.it can be integrated with your own project, both client and server applications.

This is two thing use must remember

- first I don't copy any thing in buffer when parse the message . So it make libary is faster , and easy for use to control memory  , but you will lost data if you don't dynamic alloc this data
- if your recevice a single message in time . You should you funtion in lmsrp_mess.h , is is stable , simple and fast . But for send file , i recommend use lmsrp_stream_praser() ;

using Doxygen to get Documentation
#build project
 My project can use in cross-plaform , to build project
 <pre>
 cd build
 ./configure --host=arm-zynq-linux-gnueabihf
 make
 </pre>
 the output in build/arm-zynq-linux-gnueabihf folder
# Demo code is comming son!