/**
 * @file uart.cpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../inc/uart.hpp"

int ret;
int UART::libtty_setcustombaudrate(int fd, int baudrate)
{
    struct termios2 tio;

    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = baudrate;
    tio.c_ospeed = baudrate;

    return 0;
}

int UART::libtty_setopt(int fd, int speed, int databits, int stopbits, char parity, char hardflow)
{
    struct termios newtio;
    struct termios oldtio;
    int i;

    memset(&newtio, 0, sizeof(newtio));

    if (tcgetattr(fd, &oldtio) != 0)
    {
        perror("tcgetattr");
        return -1;
    }
    newtio.c_cflag |= (CLOCAL | CREAD);
    newtio.c_cflag &= ~CSIZE;

    /* set data bits */
    switch (databits)
    {
    case 5:
        newtio.c_cflag |= CS5;
        break;
    case 6:
        newtio.c_cflag |= CS6;
        break;
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "unsupported data size\n");
        return -1;
    }

    /* set parity */
    switch (parity)
    {
    case 'n':
    case 'N':
        newtio.c_cflag &= ~PARENB; /* Clear parity enable */
        newtio.c_iflag &= ~CSTOPB; /* Disable input parity check */
        break;
    case 'o':
    case 'O':
        newtio.c_cflag |= (PARODD | PARENB); /* Odd parity instead of even */
        newtio.c_iflag |= INPCK;             /* Enable input parity check */
        break;
    case 'e':
    case 'E':
        newtio.c_cflag |= PARENB;  /* Enable parity */
        newtio.c_cflag &= ~PARODD; /* Even parity instead of odd */
        newtio.c_iflag |= INPCK;   /* Enable input parity check */
        break;
    case 'm':
    case 'M':
        newtio.c_cflag |= PARENB; /* Enable parity */
        newtio.c_cflag |= CMSPAR; /* Stick parity instead */
        newtio.c_cflag |= PARODD; /* Even parity instead of odd */
        newtio.c_iflag |= INPCK;  /* Enable input parity check */
        break;
    case 's':
    case 'S':
        newtio.c_cflag |= PARENB;  /* Enable parity */
        newtio.c_cflag |= CMSPAR;  /* Stick parity instead */
        newtio.c_cflag &= ~PARODD; /* Even parity instead of odd */
        newtio.c_iflag |= INPCK;   /* Enable input parity check */
        break;
    default:
        fprintf(stderr, "unsupported parity\n");
        return -1;
    }

    /* set stop bits */
    switch (stopbits)
    {
    case 1:
        newtio.c_cflag &= ~CSTOPB;
        break;
    case 2:
        newtio.c_cflag |= CSTOPB;
        break;
    default:
        tcsetattr(fd, TCSANOW, &newtio);
        perror("unsupported stop bits\n");
        return -1;
    }

    if (hardflow)
        newtio.c_cflag |= CRTSCTS;
    else
        newtio.c_cflag &= ~CRTSCTS;

    newtio.c_cc[VTIME] = 0; /* Time-out value (tenths of a second) [!ICANON]. */
    newtio.c_cc[VMIN] = 0;  /* Minimum number of bytes read at once [!ICANON]. */
    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) != 0)
    {
        perror("tcsetattr");
        return -1;
    }

    // /* set tty speed */
    // if (libtty_setcustombaudrate(fd, speed) != 0)
    // {
    //     perror("setbaudrate");
    //     return -1;
    // }

    return 0;
}

int UART::libtty_open(const char *devname)
{
    int fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
    int flags = 0;

    if (fd < 0)
    {
        perror("open device failed");
        return -1;
    }

    flags = fcntl(fd, F_GETFL, 0);
    flags &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
    {
        printf("fcntl failed.\n");
        return -1;
    }

    if (isatty(fd) == 0)
    {
        printf("not tty device.\n");
        return -1;
    }
    else
        printf("tty device test ok.\n");

    return fd;
}

int UART::libtty_sendbreak(int fd)
{
    return tcsendbreak(fd, 0);
}

// static int libtty_write(int fd, float *data) //
// {
//     int nwrite;                       // count
//     u_char buf[64] = {0xff, 0xfe, 1}; // start

//     memset(buf + 3, 0x00, 61);                // refrash the buf but [0],[1]
//     memcpy(buf + 3, data, 3 * sizeof(float)); // you can put the 0xfffe and 0xa0d in you data too

//     *(buf + 15) = 0xa;
//     *(buf + 16) = 0xd;

//     nwrite = write(fd, buf, 17);
//     printf("wrote %d bytesa lready.\n", nwrite);

//     return nwrite;
// }

int UART::Libtty_Write(int fd, float *data, __u8 buff)
{
    int nwrite;           // count
    u_char buf[64] = {0}; // start
    buf[0] = 0xff;
    buf[1] = 0xfe;
    buf[2] = buff;
    memset(buf + 3, 0x00, 61);                // refrash the buf but [0],[1]
    memcpy(buf + 3, data, 3 * sizeof(__s16)); // you can put the 0xfffe and 0xa0d in you data too
                                              // cout<<*data<<*(data+1)<<*(data+2)<<endl;
    for (int i = 3; i < 9; i++)
        buf[9] += buf[i];
    *(buf + 10) = 0xaa;
    *(buf + 11) = 0xdd;

    nwrite = write(fd, buf, 12);
    // sleep(0.01);
    printf("wrote %d bytesa lready.\n", nwrite);

    return nwrite;
}

#ifndef different_type
int UART::Libtty_Write(int fd, __s16 *data, __u8 buff)
{
    int nwrite;           // count
    u_char buf[64] = {0}; // start
    buf[0] = 0xff;
    buf[1] = 0xfe;
    buf[2] = buff;
    memset(buf + 3, 0x00, 61);                // refrash the buf but [0],[1]
    memcpy(buf + 3, data, 3 * sizeof(__s16)); // you can put the 0xfffe and 0xa0d in you data too
                                              // cout<<*data<<*(data+1)<<*(data+2)<<endl;
    for (int i = 3; i < 9; i++)
        buf[9] += buf[i];
    *(buf + 10) = 0xaa;
    *(buf + 11) = 0xdd;

    nwrite = write(fd, buf, 12);
    // sleep(0.01);
    printf("wrote %d bytesa lready.\n", nwrite);

    return nwrite;
}
int UART::Libtty_Write(int fd, float (*data)[3], __u8 buff, int num)
{
    int nwrite;
    int index = 0;
    u_char buf[100] = {0};
    float mixbuff[num * 3] = {0};

    buf[0] = 0xff;
    buf[1] = 0xfe;
    buf[2] = buff;
    buf[3] = (__u8)num;

    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            mixbuff[index++] = data[i][j];
        }
    }

    memset(buf + 4, 0x00, 96);
    memcpy(buf + 4, mixbuff, 3 * num * sizeof(float));

    *(buf + (num * 3) * 4 + 4) = 0xaa;
    *(buf + (num * 3) * 4 + 5) = 0xdd;

    nwrite = write(fd, buf, (num * 3) * 4 + 6);
    // sleep(0.01);
    // printf("wrote %d bytesa lready.\n", nwrite);

    return nwrite;
}

int UART::Libtty_Write(int fd, uint8_t *data, __u8 buff)
{
    int nwrite;           // count
    u_char buf[64] = {0}; // start
    buf[0] = 0xff;
    buf[1] = 0xfe;
    buf[2] = buff;
    memset(buf + 3, 0x00, 61);
    buf[3] = data[0];
    // uint8_t data1[6] = {0};
    // __s16 data2[5] = {0};
    // int index1 = 0;
    // int index2 = 0;

    // for(int i =0; i< 11; i+=2)
    // {
    //   data1[index1] =(uint8_t) data[i];
    //   index1++;
    // }
    // for(int i = 1; i<11; i+=2)
    // {
    //    data2[index2] =(__s16) data[i];
    //    index2++;
    // }

    // memset(buf + 3, 0x00, 61);
    // buf[3] = data1[0];

    // for(int i = 0; i<5; i++)
    // {
    // buf[4 + 3*i] = data1[i + 1];
    //  memcpy(&buf[4 + 3 * i], &data2[i], sizeof(__s16));
    //  memcpy(&buf[6 + 3 * i], &data1[i+1], sizeof(uint8_t));
    // }
    *(buf + 4) = 0xaa;
    *(buf + 5) = 0xdd;

    nwrite = write(fd, buf, 5);

    printf("wrote %d bytesa lready.\n", nwrite);

    return nwrite;
}
#endif

void UART::uart_init()
{
    UART uart;
    signal(SIGINT, uart.sig_handler);

    fd = libtty_open(device);
    // printf("%d", fd);
    if (fd < 0)
    {
        printf("libtty_open: %s error.\n", device);
        exit(0);
    }

    ret = libtty_setopt(fd, speed, 8, 1, 'n', hardflow);
    if (ret != 0)
    {
        printf("libtty_setopt error.\n");
        exit(0);
    }
}
// static int libtty_write(int fd, float *data) //
// {
//   int nwrite;                       // count
//   u_char buf[64] = {0}; // start
//   buf[0] = 0xff;
//   buf[1] = 0xfe;
//   buf[2] = 0x02;
//   int32_t data_s32;
//   memset(&buf[3], 0x00, 61);                // refrash the buf but [0],[1]

//   for(int i = 0; i < 3;i++)
//   {
//     data_s32 = (int32_t)data[i];
//     memcpy(&buf[3], &data[i], sizeof(float)); // you can put the 0xfffe and 0xa0d in you data too

//   }

//   *(buf + 15) = 0xa;
//   *(buf + 16) = 0xd;

//   nwrite = write(fd, buf, 17);
//   printf("wrote %d bytesa lready.\n", nwrite);

//   return nwrite;
// }

void UART::sig_handler(int signo)
{
    printf("capture sign no:%d\n", signo);
    if (fp != NULL)
    {
        fflush(fp);
        fsync(fileno(fp));
        fclose(fp);
    }
    exit(0);
}

//   if ((width[i] * height[i]) > (width[i + 1] * height[i + 1]))
void UART::libtty_receive(int fd, int &data)
{

    int RxLen = 0;
    u_char RxBuff[10] = {0};

    while (((RxLen = read(fd, RxBuff, sizeof(RxBuff))) > 0))
    {

        while (RxBuff[0] == 0xff && RxBuff[1] == 0xfe && RxBuff[5] == 0xaa && RxBuff[6] == 0xdd)
        {
            tcflush(fd, TCIOFLUSH);
            // printf("%x\n", RxBuff[0]);
            // printf("%x\n", RxBuff[1]);
            // printf("%x\n", RxBuff[2]);
            data = (int)RxBuff[2];

            memset(RxBuff, 0x00, 10);
        }
    }
}
