import torch
#import mypacks
import cv2
import numpy as np
import threading
import sys
import os

#图片
picture = np.zeros((640,480,3))

#def ok():
#    os.system('touch a.txt')
#    print('ok')


#更新图片线程
def updatePic():

    global picture
    picture=cv2.imread("/home/dxy/Downloads/c++ python/000001.jpeg")
    #循环更新
    while True:
        picture = mypacks.gmi.getInput()



#加载模型
def load_model():

    # 加载yolov5模型
    yolov5_model = torch.hub.load("/home/dxy/Downloads/yolov5-6.0",'custom', source='local', path = "/home/dxy/Downloads/yolov5-6.0/best.pt") 
    yolov5_model.conf = 0.5
    yolov5_model.classes = [0]
    print("load ok")
    return yolov5_model




def detect_pic(yolov5_model, picture):

    #进行推理
    detect_result = yolov5_model(picture)

    #结果标志符
    have_result = 0

    #每一个识别的结果
    for *box, conf, cls in detect_result.xyxy[0]:

        point1 = [(int)(box[0].item()), (int)(box[1].item())]
        point2 = [(int)(box[2].item()), (int)(box[3].item())]

        cv2.line(picture, (point1[0],point1[1]), (point1[0],point2[1]), (0,255,0), 3)
        cv2.line(picture, (point1[0],point1[1]), (point2[0],point1[1]), (0,255,0), 3)
        cv2.line(picture, (point2[0],point1[1]), (point2[0],point2[1]), (0,255,0), 3)
        cv2.line(picture, (point1[0],point2[1]), (point2[0],point2[1]), (0,255,0), 3)

        print(point1[0], point1[1], point2[0], point2[1])       # 传出消息，任务完成

        have_result = 1

    #显示图片
    cv2.imshow('img',picture)
    cv2.waitKey(1)

    #返回值
    if have_result:
        print("load ok")
        return (point1[0], point1[1], point2[0], point2[1])
    else:
        return (-1, -1, -1, -1)




def ok(cv_color1):

    #yolov5_model = load_model()
    #detect_pic(yolov5_model, cv_color)
    cv2.imshow("1",cv_color1)
    cv2.waitKey(1)






