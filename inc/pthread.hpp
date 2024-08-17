#ifndef PTHREAD_H
#define PTHREAD_H
#include "main.hpp"
#include <k4a/k4a.hpp>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>

inline std::shared_ptr<cv::Mat> matBuff;
inline std::shared_ptr<cv::Mat> depthBuff;
inline pthread_mutex_t buff_mutex;

class pthread
{
private:
public:
    static void *k4aUpdate(void *argc);
    static void *create_infer(void *argc);
    static void *create_infer_seg(void *argc);
};
#endif