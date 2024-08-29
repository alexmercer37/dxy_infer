#ifndef REALSENSE_H
#define REALSENSE_H
#include <librealsense2/rs.hpp>
#include "main.hpp"
class RealSense
{
private:
    uint16_t dev_count;
    rs2::pipeline pipe;
    rs2::config cfg;
    rs2::frameset frames;

public:
    void
    realsense_init(rs2::pipeline &pipe);
    void realsense_update(rs2::pipeline &pipe, rs2::frameset &frames);
    cv::Mat *get_realsense_depth(rs2::frameset &frames);
    cv::Mat *get_realsense_rgb(rs2::frameset &frames);
};
#endif