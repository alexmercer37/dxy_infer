/**
 * @file realsense.cpp
 * @author alexmercer37 (3416181310@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-29
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../inc/realsense.hpp"

void RealSense::realsense_init(rs2::pipeline &pipe)
{
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8, 30);
    rs2::pipeline_profile profile = pipe.start(cfg);
}

void RealSense::realsense_update(rs2::pipeline &pipe, rs2::frameset &frames)
{

    frames = pipe.wait_for_frames();
    // auto aligned_frameset = align_to_color.process(frameset);
}

cv::Mat *RealSense::get_realsense_depth(rs2::frameset &frames)
{
    rs2::depth_frame depth_stream = frames.get_depth_frame();

    const int width = depth_stream.as<rs2::video_frame>().get_width();
    const int height = depth_stream.as<rs2::video_frame>().get_height();

    cv::Mat *depth_image = new cv::Mat(cv::Size(width, height), CV_16U, (void *)depth_stream.get_data(), cv::Mat::AUTO_STEP);

    return depth_image;
}

cv::Mat *RealSense::get_realsense_rgb(rs2::frameset &frames)
{
    rs2::video_frame color_stream = frames.get_color_frame();

    const int width = color_stream.as<rs2::video_frame>().get_width();
    const int height = color_stream.as<rs2::video_frame>().get_height();

    cv::Mat *color_image = new cv::Mat(cv::Size(width, height), CV_8UC3, (void *)color_stream.get_data(), cv::Mat::AUTO_STEP);

    return color_image;
}