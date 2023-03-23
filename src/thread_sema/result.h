#ifndef __RESULT__H__
#define __RESULT__H__

#include <atomic>

#include "any.h"
#include "semaphore.h"
#include "task.h"

class Task;

class Result {
public:
  Result(std::shared_ptr<Task> sp, bool flag = true);
  ~Result();

  Any get();

  void setVal(Any any);

private:
  /// @brief 存储返回值
  Any m_any_;
  /// @brief 信号量
  Semaphore m_sema_;
  /// @brief 指向对应获取返回值的对象
  std::shared_ptr<Task> m_task_;
  /// @brief 返回值是否有效
  std::atomic_bool m_isvalid_;
};

#endif /* __RESULT__H__ */
