#include "can_send.hpp"
#include <iostream>

int s, nbytes;
struct sockaddr_can addr;
struct can_frame can_frame;
struct ifreq ifr;

void Answer_Master()
{
    can_frame.can_id = 0x00090101 | CAN_EFF_FLAG; // 扩展帧的标识
    can_frame.can_dlc = 2;
    can_frame.data[0] = 'Z';
    can_frame.data[1] = 'J';
    int nbytes = write(s, &can_frame, sizeof(struct can_frame));
}

void Send_Lagori_Coordinate(int color, int cls, int pose, float angle, float x, float y)
{
    can_frame.can_id = 0x00090101 | CAN_EFF_FLAG; // 扩展帧的标识
    can_frame.can_dlc = 8;
    can_frame.data[0] = color;
    can_frame.data[1] = cls;
    can_frame.data[2] = pose;
    can_frame.data[3] = (uint8_t)((int8_t)angle);
    can_frame.data[4] = (uint8_t)((int16_t)x >> 8);
    can_frame.data[5] = (uint8_t)((int16_t)x);
    can_frame.data[6] = (uint8_t)((int16_t)y >> 8);
    can_frame.data[7] = (uint8_t)((int16_t)y);
    int nbytes = write(s, &can_frame, sizeof(struct can_frame));
}

void Can_Init()
{
    /*******************************************************/
    // 初始化CAN接口
    const char *ifname = "can0";
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) // 创建 SocketCAN关键字
    {
        perror("Error while opening socket \n "
               "try: sudo ip link set can0 up txqueuelen 1000 type can bitrate 1000000");
    }
    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr); // 指定can0设备
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) // 将套接字与can0绑定
    {
        perror("Error in socket bind");
    }
    /*******************************************************/
    // 设置过滤规则
    struct can_filter rfilter[2];
    rfilter[0].can_id = 0x00010900;
    rfilter[0].can_mask = CAN_EFF_MASK;
    rfilter[1].can_id = 0x00020101;
    rfilter[1].can_mask = CAN_EFF_MASK;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW, &rfilter, sizeof(rfilter));
    /*******************************************************/
}