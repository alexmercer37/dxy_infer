/**
 * @file pthread.hpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef PTHREAD_H
#define PTHREAD_H

#include "main.hpp"
#include <k4a/k4a.hpp>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>
#define python_test_succeed

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
    static void *usb_camera_infer(void *argc);
    static void *detect_python(void *argc);
};
#endif