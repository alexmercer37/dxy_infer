cmake_minimum_required(VERSION 3.16)

include_directories(.)

project(pro)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/workspace)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX17_ABI=1")
set(CUDA_GEN_CODE "-gencode=arch=compute_75,code=sm_75")

set(OpenCV_INCLUDE_DIRS "/home/ddxy/Downloads/dxy_infer/inc/opencv")
set(OpenCV_LIBS_DIRS "/usr/local/lib")
set(CUDA_DIR "/usr/local/cuda-12.2")
set(CUDNN_DIR    "/usr/local/cuda-12.2")
set(TENSORRT_DIR "/home/ddxy/project/TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-12.0/TensorRT-8.6.1.6")
set(cpp_srcs  
    src/uart.cpp  
    src/main.cpp
    src/camera.cpp
    src/detect.cpp
    src/pthread.cpp
)  

find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)
find_package(k4a REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
    ${CUDA_DIR}/include
    ${TENSORRT_DIR}/include
    ${CUDNN_DIR}/include
    inc
    )

link_directories(
    ${OpenCV_LIBS}
    ${CUDA_DIR}/lib64
    ${CUDNN_DIR}/lib64
    ${TENSORRT_DIR}/lib
    ${PROJECT_SOURCE_DIR}/workspace
    )

file(GLOB_RECURSE cpp_srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(pro ${cpp_srcs})

target_link_libraries(pro  libcu.a)
target_link_libraries(pro  nvinfer)
target_link_libraries(pro  cuda cublas cudart cudnn)
target_link_libraries(pro  pthread)
target_link_libraries(pro  ${OpenCV_LIBS})
target_link_libraries(pro  k4a::k4a)
# add_custom_target(
#         yolo
#         DEPENDS camera
#         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/workspace
#         COMMAND ../pro yolo
# )

