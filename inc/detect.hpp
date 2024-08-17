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

static const char *cocolabels[] = {"person", "bicycle", "car",
                                   "motorcycle", "airplane", "bus",
                                   "train", "truck", "boat",
                                   "traffic light", "fire hydrant", "stop sign",
                                   "parking meter", "bench", "bird",
                                   "cat", "dog", "horse",
                                   "sheep", "cow", "elephant",
                                   "bear", "zebra", "giraffe",
                                   "backpack", "umbrella", "handbag",
                                   "tie", "suitcase", "frisbee",
                                   "skis", "snowboard", "sports ball",
                                   "kite", "baseball bat", "baseball glove",
                                   "skateboard", "surfboard", "tennis racket",
                                   "bottle", "wine glass", "cup",
                                   "fork", "knife", "spoon",
                                   "bowl", "banana", "apple",
                                   "sandwich", "orange", "broccoli",
                                   "carrot", "hot dog", "pizza",
                                   "donut", "cake", "chair",
                                   "couch", "potted plant", "bed",
                                   "dining table", "toilet", "tv",
                                   "laptop", "mouse", "remote",
                                   "keyboard", "cell phone", "microwave",
                                   "oven", "toaster", "sink",
                                   "refrigerator", "book", "clock",
                                   "vase", "scissors", "teddy bear",
                                   "hair drier", "toothbrush"};
class Detect
{
private:
    cv::Mat *image;
    std::shared_ptr<yolo::Infer> yolo;

public:
    void time();
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