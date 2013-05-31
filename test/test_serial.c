//test_serial.c
//performs simple tests serial library
//these tests use can virtual serial ports
//using the virtual-serial.sh script


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <check.h> //unit testing framework

#include "../serial/serial.h"

//file descriptors for handling serial ports
//they are global so all test functions can use it
int file_descr1;
int file_descr2;

//put devices for test here
#define DEVICE1 "/dev/pts/2"
#define DEVICE2 "/dev/pts/4"

//this function is called before each test function
void setup(void){
    file_descr1 = serial_create(DEVICE1, B9600);
    file_descr2 = serial_create(DEVICE2, B9600);
}
//this function is called after each test function
void teardown(void){
    serial_close(file_descr1);
    serial_close(file_descr2);
}
//test initializing serial port
START_TEST(test_serial_create){
    ck_assert_msg(file_descr1 != -1, "could not open device %s", DEVICE1);
    ck_assert_msg(file_descr2 != -1, "could not open device %s", DEVICE2);
}
END_TEST

//test writing data
START_TEST(test_serial_write){
    //data to send
    byte data[5] = {0xFF, 0x08, 0x00, 0xFD, 0x88};
    //5 bytes should be  sent
    ck_assert(serial_write(file_descr1,data, 5) == 5);
}
END_TEST
//test reading data
START_TEST(test_serial_read){
    //data to send
    byte data1[5] = {0xFF, 0x08, 0x00, 0xFD, 0x88};
    byte buffer[20]; //buffer for receiving
    bzero(buffer, sizeof(buffer)); //set buffer to all zeros
    
    //write some data to port at file_descr1
    serial_write(file_descr1, data1, 5); 
    usleep(10000); //wait for data to be stored in serial buffer

    //check that read went correctly
    ck_assert(serial_read(file_descr2, buffer, 5) == 5);
    ck_assert(memcmp(data1, buffer, 5) == 0); //data and buffer should match;
}
END_TEST
//test getting data available at port
START_TEST(test_serial_available){
    //data to send
    byte data[5] = {0xFF, 0x08, 0x00, 0xFD, 0x88};
    serial_write(file_descr1,data, 5);
    usleep(10000);//wait for data to be stored in serial buffer
    ck_assert(serial_available(file_descr2) == 5);//5 bytes should be available
}
END_TEST
//test clearing serial buffer
START_TEST(test_serial_clear){
    //data to send
    byte data[5] = {0xFF, 0x08, 0x00, 0xFD, 0x88};

    //write 5 bytes to buffer then clear it
    serial_write(file_descr1,data, 5);
    serial_clear(file_descr1);

    //check that buffer is cleared
    ck_assert(serial_available(file_descr1) == 0);
}
END_TEST

//testing suite
//put all testing functions here
Suite* serial_suite(void){
    Suite *s = suite_create("serial");

    //core test cases
    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_serial_create);
    tcase_add_test(tc_core, test_serial_write);
    tcase_add_test(tc_core, test_serial_read);
    tcase_add_test(tc_core, test_serial_available);
    tcase_add_test(tc_core, test_serial_clear);
    suite_add_tcase(s, tc_core); //add test cases to suite

    return s;
}
int main(){
    int number_failed;
    Suite *s = serial_suite();
    SRunner *sr = srunner_create(s);

    //run all the tests
    srunner_run_all(sr, CK_VERBOSE);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
