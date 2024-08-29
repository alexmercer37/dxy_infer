/**
 * @file queue.hpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "main.hpp"

#define MAXSIZE 100
using namespace std;

template <typename T>
class Queue
{
private:
    T *queue;
    // std::vector<T> queue;
    //! 利用c++标准库容器进行自动内存分配
    int front, rear;

public:
    Queue()
    {
        queue = new void *[MAXSIZE];
        front = rear = -1;
    }

    ~Queue()
    {
        delete[] queue;
    }
    bool empty()
    {
        return (front == rear);
    }
    bool isfull()
    {
        if ((rear + 1) % (MAXSIZE) == front)
            return 1;

        return (front == rear);
    }
    void push(T data)
    {
        if (rear == MAXSIZE - 1)
            return;
        rear = (rear + 1) % MAXSIZE;
        queue[rear] = data;
    }
    void pop()
    {
        if (empty())
            return;
        if (front == rear)
            return;
        front = (front + 1) % MAXSIZE;
        queue[front] = nullptr;
    }
    T getfront()
    {
        if (front == rear)
            return nullptr;
        int head = front + 1;
        T data = queue[head];
        return data;
    }
    T getdata(int num)
    {
        return queue[(front + num) % MAXSIZE];
    }
};
#endif
