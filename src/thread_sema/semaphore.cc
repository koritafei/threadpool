#include "semaphore.h"

#include <iostream>

Semaphore::Semaphore(int reslimit) : m_res_limits_(reslimit) {
}

Semaphore::~Semaphore() {
}

void Semaphore::wait() {
  std::unique_lock<std::mutex> lock{m_sema_mutex_};
  m_cond_.wait(lock, [&]() -> bool {
    return m_res_limits_ > 0;
  });
  m_res_limits_--;
}

void Semaphore::post() {
  std::unique_lock<std::mutex> lock{m_sema_mutex_};
  m_res_limits_++;
  m_cond_.notify_all();
}
