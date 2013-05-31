#include<unistd.h> //POSIX functions for reading and writing
#include<stdio.h> //output errors to stderr
#include <fcntl.h> //constants for controlling a file
#include <string.h> //memset
#include "sys/ioctl.h" //ioctl
#include "serial.h"

int serial_create(const char* device, speed_t baud_rate){
    serial_options_t options;//options for serial port

    //file descriptor for handling device
    int fd; 

    //flags for opening device
    //O_RDWR     - open for read write
    //O_NOCTTY   - not a controlling terminal
    //O_NONBLOCK - return from reading immediately
    const int open_flags = (O_RDWR | O_NOCTTY | O_NONBLOCK);

    //open the port with the provided flags
    if((fd = open(device,  open_flags)) ==  -1){
        fprintf(stderr, "Unable able to open device %s\n.", device);
        return -1;
    }

    //set file descriptor flags as modifiable
    fcntl (fd, F_SETFL, O_RDWR) ;

    //clear all settings 
    memset(&options, 0, sizeof(options));

    //get options from file descriptor
    if(tcgetattr(fd, &options) == -1){
        perror("unable to retrieve serial options from device. Is the device connected?");
    };

    cfmakeraw(&options); //handle only raw data

    //set baud rate
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    //initialize options to default values
    // - no processing for input/output/lines/characters
    // - no parity bits
    options.c_iflag=0;
    options.c_oflag=0;
    options.c_cflag=CS8|CREAD|CLOCAL;// 8n1
    options.c_lflag=0;

    //return from read immediately, no waiting for min amount of bytes
    //time out from waiting for read after 10seconds
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 100;//deciseconds; 100/10 = 10sec

    //apply settings
    if(tcsetattr(fd, TCSANOW | TCSAFLUSH, &options) == -1){
        perror("unable to set default serial options for device. Is the device connected?");
        return -1;
    };

    int status; //status for terminal;

    //apply data terminal ready and
    //request to send to port
    ioctl (fd, TIOCMGET, &status);
    status |= TIOCM_DTR ;
    status |= TIOCM_RTS ;
    ioctl (fd, TIOCMSET, &status);

    usleep(10000); //wait a bit for options to be stored

    return fd;
}
void serial_set_baud_rate(const int fd, const speed_t baud_rate){
     serial_options_t options;

     //get the settings for current serial port
     tcgetattr(fd, &options);

     //set baud rate
     cfsetispeed(&options, baud_rate);
     cfsetospeed(&options, baud_rate);

     //apply changes now
     tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);
     usleep(10000);
}
int serial_available(const int fd){
   int available; //amount of bytes available for read

   //check for error
   if(ioctl(fd,FIONREAD, &available) == -1)return -1;

   return available;
}
void serial_clear(const int fd){
    tcflush(fd, TCIOFLUSH);
}
speed_t serial_get_baud_rate(const int fd) {
    serial_options_t options;
    memset(&options, 0, sizeof(options));
    tcgetattr(fd, &options);
    return cfgetospeed(&options);
}
int serial_write(const int fd, const byte* bytes, const int amount){
    return write(fd, bytes, amount); 
}
int serial_write_val(const int fd, const byte b){
    return write(fd, &b, 1);
}
int serial_read(const int fd, byte* buffer, const int size){
    return read(fd, buffer, size);
}
int serial_read_val(const int fd, byte* value){
    return read(fd, value, 1);
}
void serial_close(const int fd){
    close(fd);
}

