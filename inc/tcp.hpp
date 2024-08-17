/**
 * @file tcp.hpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TCP_H_
#define __TCP_H_

#include "main.hpp"
namespace TCP
{
    class Addr
    {
    public:
        Addr(char *ip, int port);
        const char *getIP();
        int getPort();

    private:
        int port;
        const char *ip;
    };

    class Tcp
    {
    public:
        int connetToServe(Addr &addr, int &clientfd);
        int Init_Socket(Addr &addr);
        int Get_Socket(Addr &addr);
        int lisen(int &listenfd);

    private:
    };

    class TCP_Manage : public Tcp
    {
    public:
        int Accept(int &listenfd);
        int TCP_manage(int &clientfd);
        int Recv(int &socketNum, void *RECV_DATA, size_t size);
        int Send(int clientfd, void *SEND_DATA, size_t size);
        void Close(int &listenfd);
        static void *tcp_send(void *argc);
        static void *tcp_recv(void *argc);

    private:
    };
};

#endif
