cmake_minimum_required(VERSION 3.16)

include_directories(.)

# project(cu)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/workspace)

# set(SMS "60;61;62;70;72;75;86")   
# set(GENCODE_FLAGS "")  
  
# foreach(sm ${SMS})  
#     set(CUDA_GEN_CODE "-gencode=arch=compute_${sm},code=sm_${sm}")  
#     list(APPEND GENCODE_FLAGS ${CUDA_GEN_CODE})  
# endforeach()  
set(CUDA_GEN_CODE "-gencode=arch=compute_75,code=sm_75")
enable_language(CUDA)

# set(CUDA_TOOLKIT_ROOT_DIR     "/usr/lib/nvidia-cuda-toolkit/bin")
set(CUDA_DIR "/usr/local/cuda-12.2")
set(CUDNN_DIR    "/usr/local/cuda-12.2")
set(TENSORRT_DIR "/home/ddxy/project/TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-12.0/TensorRT-8.6.1.6")
set(cuda_srcs  
infer.cu
yolo.cu
)  

find_package(CUDA QUIET REQUIRED)
# find_package(OpenCV REQUIRED)


include_directories(
    ${PROJECT_SOURCE_DIR}
    # ${OpenCV_INCLUDE_DIRS}
    ${CUDA_DIR}/include
    ${TENSORRT_DIR}/include
    ${CUDNN_DIR}/include
    inc
    # ${CUDA_TOOLKIT_ROOT_DIR}
    )


link_directories(
    # ${OpenCV_LIBS_DIRS}
    ${CUDA_DIR}/lib64
    ${CUDNN_DIR}/lib64
    ${TENSORRT_DIR}/lib
    )


set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11 -O0 -Xcompiler -fPIC -g -w ${CUDA_GEN_CODE}")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -allow-unsupported-compiler")
# file(GLOB_RECURSE cuda_srcs ${PROJECT_SOURCE_DIR}/src/*.cu)

cuda_add_library(cu STATIC ${cuda_srcs})
target_compile_features(cu PUBLIC cxx_std_11)
set_target_properties(cu
                       PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


target_link_libraries(cu nvinfer nvinfer_plugin)
target_link_libraries(cu cuda cublas cudart cudnn)
target_link_libraries(cu)
