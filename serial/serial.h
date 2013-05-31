//serial.h
//a simple library for handling serial devices

#ifndef _SERIAL_H
#define _SERIAL_H
#include <termios.h> //for configuring serial terminal
#include <stdint.h> //uint8_t

//type of data for transactions
typedef uint8_t byte; 
//type to handle serial options
typedef struct termios serial_options_t;

/*
 *Initializes Serial port with baud with
 *The file descriptor for the serial port is returned
 *    device = path to serial device; e.g. "/dev/ttyS1"
 *    baud_rate = the baud rate for the port
 */
int serial_create(const char* device, const speed_t baud_rate);
/*
 *Sets the baud rate for the serial port accessible via file descriptor
 *   fd = file descriptor for the port
 *   baud_rate =  new baud rate for the port
 */
void serial_set_baud_rate(const int fd, const speed_t baud_rate);
/**
 *Gets the baud rate for a port
 *  fd = file descriptor for the port
 */
speed_t serial_get_baud_rate(const int fd);
/**
 *Writes an array of bytes to serial port
 *  fd = file descriptor for the port
 *  bytes = array of bytes to write to port
 *  amount = amount of bytes from bytes to write to port (usually strlen(bytes))
 */
int serial_write(const int fd, const byte* bytes, const int amount);
/*Writes a single byte to serial buffer
 *  fd = file descriptor for port
 *  b = byte to write to serial buffer
 *returns total amount of bytes written to buffer or -1 if error occurs
 */
int serial_write_val(const int fd, const byte b);
/**
 *Reads data from the serial port
 *  fd = file descriptor for the port
 *  buffer = where data read from the serial buffer will be stored
 *  size = how many bytes to read from serial buffer 
 *returns total amount of bytes read
 */
int serial_read(const int fd, byte* buffer, const int size);
/**
 *Reads a single byte from serial buffer
 *  fd = file descripter for the port
 *  value = holds byte read from buffer
 *returns 1 if byte was read, 0 = no bytes read, -1 = error
 */
int serial_read_val(const int fd, byte* value);
/**
 *Returns how many bytes are available to be read from serial port
 *  fd = file descriptor for the port
 */
int serial_available(const int fd);
/**
 *Clears serial buffers
 *  fd = file descriptor for the serial port
 */
void serial_clear(const int fd);
/**
 *Closes serial port
 *  fd = file descriptor for the serial port
 */
void serial_close(const int fd);
#endif //_SERIAL_H
