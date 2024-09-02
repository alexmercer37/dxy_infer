#ifndef _CAN_SEND_H
#define _CAN_SEND_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdint-gcc.h>

extern int s, nbytes;
extern struct can_frame can_frame;

void Can_Init();
void Answer_Master();
void Send_Lagori_Coordinate(int color, int cls, int pose, float angle, float x, float y);

#endif