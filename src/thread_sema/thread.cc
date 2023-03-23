#include "thread.h"

#include <thread>

std::size_t Thread::m_gen_id_ = 0;

Thread::Thread(threadFunc func)
    : m_thread_func_(func), m_thread_id_(m_gen_id_++) {
}

Thread::~Thread() {
}

std::size_t Thread::getThreadId() const {
  return m_thread_id_;
}

void Thread::start() {
  std::thread t(m_thread_func_, m_thread_id_);
  t.detach();
}
