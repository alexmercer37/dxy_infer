
#include "../inc/main.hpp"

using namespace std;

int main()
{

  pthread_mutex_init(&buff_mutex, nullptr);

  matBuff.reset(new cv::Mat);
  depthBuff.reset(new cv::Mat);

  pthread_t threads[10] = {0};

  pthread_create(&threads[0], NULL, pthread::k4aUpdate, NULL);
  pthread_create(&threads[1], NULL, pthread::create_infer, NULL);
  pthread_create(&threads[1], NULL, pthread::create_infer_seg, NULL);

  while (1)
  {
    usleep(100);
  }
  return 0;
}