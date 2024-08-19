/**
 * @file detect.hpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __INFER_H__
#define __INFER_H__

#include "main.hpp"
#define use_random_color

static const char *cocolabels[] = {"red_ball", "blue_ball"};

class Detect
{
private:
    cv::Mat *image;
    std::shared_ptr<yolo::Infer> yolo;

public:
    void Time();
    void perf();
    void batch_inference();
    std::shared_ptr<cv::Mat> single_inference(std::shared_ptr<cv::Mat> image, std::shared_ptr<yolo::Infer> yolo);
    cv::Mat *seg_inference(std::shared_ptr<cv::Mat> image, std::shared_ptr<yolo::Infer> yolo);

    void setYolo(std::shared_ptr<yolo::Infer> new_yolo)
    {
        yolo = new_yolo;
    };
};
#endif