#include "any.h"
#include "mytask.h"
#include "threadpool.h"

int main() {
  ThreadPool pool;
  pool.setThreadPoolMode(ThreadPoolMode::THREAD_MODE_CACHED);
  pool.start(std::thread::hardware_concurrency());

  // linux上，这些Result对象也是局部对象，要析构的！！！
  Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100000000));
  Result res2 = pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));

  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
  pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));

  Any d(12.34);
  std::cout << d.cast_<double>() << std::endl;

  int64_t t1 = res1.get().cast_<int64_t>();
  int64_t t2 = res2.get().cast_<int64_t>();

  std::cout << t1 << "\t" << t2 << "\t" << t1 + t2 << std::endl;
}
