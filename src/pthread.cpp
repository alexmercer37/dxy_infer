#include "../inc/main.hpp"
#include "../inc/pthread.hpp"

Camera *camera = new Camera;
Detect *detect = new Detect;

cv::Mat rgb_frame, depth_frame;
cv::Mat *rgb_ptr = new cv::Mat;
cv::Mat *depth_ptr = new cv::Mat;

k4a::capture capture;
k4a::device Device;
k4a::transformation k4aTransformation;
k4a::calibration k4aCalibration;

void *pthread::k4aUpdate(void *argc)
{
    camera->init_kinect(Device, capture, k4aTransformation, k4aCalibration);
    // int frame_count = 0;
    while (1)
    {
        // std::ostringstream filename;

        // filename << "/home/nf/Downloads/getpicture/1/frame_" << std::setw(5) << std::setfill('0') << frame_count++ << ".jpg";

        camera->picture_update(Device, capture);

        pthread_mutex_lock(&buff_mutex);

        rgb_ptr = camera->getpicture(Device, capture, rgb_frame, k4aTransformation);
        // depth_ptr = camera->getdepth(Device, capture, depth_frame, k4aTransformation);

        // cv::imwrite(filename.str(), *rgb_ptr);

        // camera->getAngel(Device);

        pthread_mutex_unlock(&buff_mutex);
        cv::cvtColor(*rgb_ptr, *rgb_ptr, cv::COLOR_BGRA2BGR);
        pthread_mutex_lock(&buff_mutex);

        matBuff = std::make_shared<cv::Mat>(rgb_ptr->clone());
        // depthBuff = std::make_shared<cv::Mat>(depth_ptr->clone());

        pthread_mutex_unlock(&buff_mutex);

        if (matBuff->empty())
        {
            std::cout << "error" << std::endl;
        }

        rgb_ptr->release();
        // depth_ptr->release();

        usleep(100);
    }
    pthread_exit(NULL);
}

void *pthread::create_infer(void *argc)
{
    auto yolo = yolo::load("/home/ddxy/下载/dxy_infer-master/yolov8n.transd.engine", yolo::Type::V8);
    if (yolo == nullptr)
        return nullptr;

    std::shared_ptr<cv::Mat> output;
    detect->setYolo(yolo);

    while (1)
    {
        while (!matBuff->empty())
        {
            pthread_mutex_lock(&buff_mutex);
            output = detect->single_inference(matBuff, yolo);
            pthread_mutex_unlock(&buff_mutex);
            usleep(100);

            // pthread_mutex_lock(&buff_mutex);
            cv::imshow("red", *output);
            cv::waitKey(1);
            // pthread_mutex_unlock(&buff_mutex);
            // usleep(100);
        }
    }
    pthread_exit(NULL);
}
void *pthread::create_infer_seg(void *argc)
{
    auto yolo = yolo::load("/home/ddxy/下载/dxy_infer-master/yolov8n-seg.b1.transd.engine", yolo::Type::V8Seg);
    if (yolo == nullptr)
        return nullptr;

    detect->setYolo(yolo);
    cv::Mat *output;

    while (1)
    {
        while (!matBuff->empty())
        {
            pthread_mutex_lock(&buff_mutex);
            output = detect->seg_inference(matBuff, yolo);
            pthread_mutex_unlock(&buff_mutex);
            usleep(100);

            if (output != nullptr)
            {
                cv::imshow("seg", *output);
                cv::waitKey(1);
            }
        }

        usleep(100);
    }
    pthread_exit(NULL);
}
