/**
 * @file pthread.cpp
 * @author alexmercer37 (3450141407@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../inc/main.hpp"
#include "../inc/pthread.hpp"
#include <python3.12/Python.h>
// #include <numpy/arrayobject.h>

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
            std::cout << "Error: matBuff is empty!" << std::endl;
        }

        rgb_ptr->release();
        // depth_ptr->release();

        usleep(100);
    }
    pthread_exit(NULL);
}

void *pthread::create_infer(void *argc)
{
    auto yolo = yolo::load("yolov8n.transd.engine", yolo::Type::V8);
    if (yolo == nullptr)
    {
        std::cout << "Loading yolo failed" << std::endl;
        return nullptr;
    }

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
            cv::imshow("k4a", *output);
            cv::waitKey(1);
            // pthread_mutex_unlock(&buff_mutex);
            // usleep(100);
        }
    }
    pthread_exit(NULL);
}

void *pthread::create_infer_seg(void *argc)
{
    auto yolo = yolo::load("yolov8n-seg.b1.transd.engine", yolo::Type::V8Seg);
    if (yolo == nullptr)
    {
        std::cout << "Loading yolo_seg failed" << std::endl;
        return nullptr;
    }

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
            else
                std::cout << "Error: seg_output is empty!" << std::endl;
        }

        usleep(100);
    }
    pthread_exit(NULL);
}

void *pthread::usb_camera_infer(void *argc)
{
    auto yolo = yolo::load("yolov8n.transd.engine", yolo::Type::V8);
    if (yolo == nullptr)
    {
        std::cout << "Loading usb_yolo failed" << std::endl;
        return nullptr;
    }

    cv::VideoCapture capture(2);
    detect->setYolo(yolo);

    std::shared_ptr<cv::Mat> output;
    std::shared_ptr<cv::Mat> frame;
    frame.reset(new cv::Mat);

    while (1)
    {

        capture.read(*frame);

        pthread_mutex_lock(&buff_mutex);
        output = detect->single_inference(frame, yolo);
        pthread_mutex_unlock(&buff_mutex);
        usleep(100);

        if (output != nullptr)
        {
            cv::imshow("USB_CAMERA", *output);
            cv::waitKey(1);
        }
        else
            std::cout << "Error: usb_camera_output is empty!" << std::endl;
    }
    pthread_exit(NULL);
}

#ifndef python_test_succeed

void *pthread::detect_python(void *argc)
{
    Py_Initialize();
    _import_array();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(\'/home/dxy/Downloads/yolov5-6.0_two_labels\')");
    PyRun_SimpleString("sys.path.append(\'.\')");
    PyRun_SimpleString("import cv2");
    PyRun_SimpleString("import numpy");
    PyRun_SimpleString("import torch");
    PyRun_SimpleString("import time");
    PyRun_SimpleString("import serial");

    PyObject *m_PyModule = PyImport_ImportModule("detect_ball");
    PyObject *m_PyDict = PyModule_GetDict(m_PyModule);
    PyObject *load_model = PyDict_GetItemString(m_PyDict, "load_model");
    PyObject *model = PyObject_CallObject(load_model, NULL);

    while (1)
    {
        if (load_model != NULL)
        {
            pthread_mutex_lock(&buff_mutex);
            int r = (*matBuff).rows;
            int c = (*matBuff).cols;
            int chnl = (*matBuff).channels();
            int nElem = r * c * chnl;

            uchar *m = new uchar[nElem];

            std::memcpy(m, (*matBuff).data, nElem * sizeof(uchar));

            npy_intp mdim[] = {r, c, chnl};

            PyObject *mat = (PyObject *)PyArray_SimpleNewFromData(chnl, mdim, NPY_UINT8, (void *)m);
            pthread_mutex_unlock(&buff_mutex);

            PyObject *detect_fun = PyDict_GetItemString(m_PyDict, "detect");
            PyObject *detect_args = PyTuple_Pack(2, model, mat);
            PyObject *result = PyObject_CallObject(detect_fun, detect_args);

            // if (PyList_Check(result))
            // {
            //     float SizeOfList = PyList_Size(result);
            //     for (int i = 0; i < SizeOfList; i++)
            //     {
            //         PyObject *ListItem = PyList_GetItem(result, i);
            //         cout << ListItem << endl;
            //         Py_DECREF(ListItem);
            //     }
            // }

            Py_XDECREF(mat);
            Py_XDECREF(result);
            Py_XDECREF(detect_args);

            delete[] m;
        }
    }
}
#endif