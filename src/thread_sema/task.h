#ifndef __TASK__H__
#define __TASK__H__

#include <iostream>

#include "any.h"
#include "result.h"

class Result;

class Task {
public:
  virtual Any run() = 0;

  void exec();

  void setResult(Result *res);

  Task();
  ~Task();

  Task(Task &&)      = delete;
  Task(const Task &) = delete;

  Task &operator=(Task &&)      = delete;
  Task &operator=(const Task &) = delete;

private:
  Result *m_result_;
};

#endif /* __TASK__H__ */
