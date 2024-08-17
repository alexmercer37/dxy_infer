/**
 * @file uart.hpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <getopt.h>
#include <vector>
#include <linux/serial.h>
#define termios asmtermios
#include <asm/termios.h>
#undef termios
#include <termios.h>

#define different_type
static const char *device = "/dev/myttyUSB";
static int speed = B115200;
static int hardflow = 0;
static int verbose = 0;
static FILE *fp;

extern int fd;
extern int ret;
extern char c;

class UART
{
public:
    UART(int *d = nullptr) : data(d) {}
    int *data;
    int libtty_setcustombaudrate(int fd, int baudrate);
    int libtty_setopt(int fd, int speed, int databits, int stopbits, char parity, char hardflow);
    int libtty_open(const char *devname);
    int libtty_sendbreak(int fd);
    static void sig_handler(int signo);
    void uart_init();
    void libtty_receive(int fd, int &data);
    int Libtty_Write(int fd, float *data, __u8 buff);

#ifndef different_type
    int Libtty_Write(int fd, float (*data)[3], __u8 buff, int num);
    int Libtty_Write(int fd, __s16 *data, __u8 buff);
    int Libtty_Write(int fd, uint8_t *data, __u8 buff);
#endif

private:
    int fd;
    int baudrate;
    int databits;
    int speed;
    int stopbits;
    char parity;
    char hardflow;
    const char *devname;
    int signo;
    __u8 buff;
    int num;

#ifndef different_type
    float (*data)[3];
    uint8_t *data;
    __s16 *data;
#endif
};
