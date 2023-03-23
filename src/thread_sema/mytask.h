#ifndef __MYTASK__H__
#define __MYTASK__H__

#include <thread>

#include "task.h"

class MyTask : public Task {
public:
  MyTask(int64_t begin, int64_t end) : begin_(begin), end_(end) {
  }

  Any run()  // run方法最终就在线程池分配的线程中去做执行了!
  {
    int64_t sum = 0;
    for (int64_t i = begin_; i <= end_; i++) {
      sum += i;
    }
    // std::cout << "tid:" << std::this_thread::get_id() << " end!, sum = " <<
    // sum
    //           << std::endl;

    return sum;
  }

private:
  int64_t begin_;
  int64_t end_;
};

#endif /* __MYTASK__H__ */
