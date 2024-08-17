#include "../inc/tcp.hpp"
using namespace TCP;

Tcp tcp;
TCP_Manage manage;

Addr::Addr(char *ip, int port)
{
    this->port = port;

    size_t size = strlen(ip) + (size_t)1;

    this->ip = (const char *)malloc(size);

    memcpy((void *)this->ip, ip, size);
}

const char *Addr::getIP()
{
    return this->ip;
}

int Addr::getPort()
{
    return this->port;
}

int Tcp::Init_Socket(Addr &addr)
{

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1)
    {
        std::cout << "create socket failed" << std::endl;
        return -1;
    }

    struct sockaddr_in bindaddr;

    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = inet_addr(addr.getIP());
    bindaddr.sin_port = htons(addr.getPort());

    bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr));

    int on = 1;

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    return listenfd;
}

int Tcp::Get_Socket(Addr &addr)
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    if (clientfd == -1)
    {
        std::cout << "create socket failed" << std::endl;
        return -1;
    }
    else
        return clientfd;
}

int Tcp::connetToServe(Addr &addr, int &clientfd)
{
    sockaddr_in serveraddr;

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(addr.getIP());
    serveraddr.sin_port = htons(addr.getPort());

    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {

        std::cout << "connect socket error" << std::endl;
        return -1;
    }
    return 0;
}

int Tcp::lisen(int &listenfd)
{
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error" << std::endl;
        return -1;
    }
    return 0;
}

int TCP_Manage::TCP_manage(int &listenfd)
{
    int socketNum = Accept(listenfd);

    return socketNum;
}

int TCP_Manage::Accept(int &listenfd)
{

    sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);

    int socketNum = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);

    return socketNum;
}

int TCP_Manage::Send(int clientfd, void *SEND_DATA, size_t size)
{
    int ret = send(clientfd, SEND_DATA, size, 0);

    if (ret != size)
    {
        std::cout << "send data failed" << std::endl;
        return -1;
    }
    // std::cout << "successfully" << SEND_DATA << std::endl;
    return 0;
}

int TCP_Manage::Recv(int &socketNum, void *RECV_DATA, size_t size)
{
    int ret = recv(socketNum, RECV_DATA, size, 0);

    if (ret > 0)
    {
        std::cout << "Receive data from the client:" << RECV_DATA << std::endl;
    }
    else
    {
        std::cout << "failed" << RECV_DATA << std::endl;
    }
    return 0;
}

void TCP_Manage::Close(int &listenfd)
{
    close(listenfd);
}

void *TCP_Manage::tcp_send(void *argc)
{
    Addr addr((char *)"192.168.3.171", 25000);
    const char *diaomao = "diaomao";

    int clientfd = tcp.Get_Socket(addr);

    tcp.connetToServe(addr, clientfd);

    while (1)
    {
        manage.Send(clientfd, (char *)diaomao, sizeof(diaomao));
    }

    manage.Close(clientfd);
}

void *TCP_Manage::tcp_recv(void *argc)
{
    Addr addr((char *)"192.168.3.61", 30000);
    int recvBuf[100] = {0};

    int listenfd = tcp.Init_Socket(addr);

    tcp.lisen(listenfd);

    int socketNum = manage.TCP_manage(listenfd);

    while (1)
    {
        manage.Recv(socketNum, recvBuf, 100);
    }
    manage.Close(socketNum);
}
