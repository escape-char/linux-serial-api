### Serial Tests README
These tests use virtual serial ports. Before running these tests you need the following
* socat  - for setting up the virtual terminal
* screen - for displaying results of socat (or whatever one prefers)
* check - c unit testing library
These programs/libraries are most likely available via a package manager for your linux manager (yum, apt-get). 

To perform the tests, do the following steps
1. cd into serials/test and run './virtual-serial.sh'. You may have to give your self permission ('chmod +x virtual-serial.sh'). This sets up virtual serial ports to test with
2. change the devices constants in test_serial.c to the output. Then run make. 
