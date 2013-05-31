#virtual-serial.sh
#this script will setup two virtual serial ports
#this is useful testing serial programs without
#a physical device

#its output will show the location of ports
#usually /dev/pts/n, where n is the port number
#leave the program running until finished testing

screen socat -v -d -d pty,raw,echo=0 pty,raw,echo=0
