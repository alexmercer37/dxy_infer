/*
 * @Author: Tommy0929
 * @Date: 2024-01-25 20:01:23
 * @LastEditors: Tommy0929 tommy07210728@163.com
 * @FilePath: /final/inc/uart.h
 * WHUROBOCON_SAVED!!!
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
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

static const char *device = "/dev/myttyUSB";
static int speed = B115200;
static int hardflow = 0;
static int verbose = 0;
static FILE *fp;

extern int fd;  // put in main is ok too (30 2 32)
extern int ret; //
extern char c;  // for test in the while

/**
 * libtty_setcustombaudrate - set baud rate of tty device
 * @fd: device handle
 * @speed: baud rate to set
 *
 * The function return 0 if success, or -1 if fail.
 */
int libtty_setcustombaudrate(int fd, int baudrate);

/**
 * libtty_setopt - config tty device
 * @fd: device handle
 * @speed: baud rate to set
 * @databits: data bits to set
 * @stopbits: stop bits to set
 * @parity: parity to set
 * @hardflow: hardflow to set
 *
 * The function return 0 if success, or -1 if fail.
 */
int libtty_setopt(int fd, int speed, int databits, int stopbits, char parity, char hardflow);

/**
 * libtty_open - open tty device
 * @devname: the device name to open
 *
 * In this demo device is opened blocked, you could modify it at will.
 */
int libtty_open(const char *devname);

/**
 * libtty_sendbreak - uart send break
 * @fd: file descriptor of tty device
 *
 * Description:
 *  tcsendbreak() transmits a continuous stream of zero-valued bits for a specific duration, if the terminal
 *  is using asynchronous serial data transmission. If duration is zero, it transmits zero-valued bits for
 *  at least 0.25 seconds, and not more that 0.5 seconds. If duration is not zero, it sends zero-valued bits
 *  for some implementation-defined length of time.
 *
 *  If the terminal is not using asynchronous serial data transmission, tcsendbreak() returns without taking
 *  any action.
 */
int libtty_sendbreak(int fd);

/**
 * libtty_write - write data to uart
 * @fd: file descriptor of tty device
 *
 * The function return the number of bytes written if success, others if fail.
 */
int libtty_write(int fd, __s16 *data, __u8 buff);
int libtty_Write(int fd, uint8_t *data, __u8 buff);

// static int libtty_write(int fd); // use char to test

static void sig_handler(int signo);

void uart_init();
void libtty_receive(int fd, int &data);
int Libtty_Write(int fd, float (*data)[3], __u8 buff, int num);
