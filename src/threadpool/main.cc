#include "threadpool.h"

int sum1(int a, int b) {
  return a + b;
}

int sum2(int a, int b, int c) {
  return a + b + c;
}

int main() {
  ThreadPool pool;
  pool.setThreadPoolMode(ThreadPoolMode::THREAD_MODE_CACHED);
  pool.start(std::thread::hardware_concurrency());

  pool.submitTask(sum1, 1, 2);
  pool.submitTask(sum2, 1, 2, 3);
}
