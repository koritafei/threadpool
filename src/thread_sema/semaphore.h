#ifndef __SEMAPHORE__H__
#define __SEMAPHORE__H__

#include <condition_variable>
#include <mutex>

class Semaphore {
public:
  Semaphore(int reslimit = 0);
  ~Semaphore();

  Semaphore(Semaphore &&)      = delete;
  Semaphore(const Semaphore &) = delete;

  Semaphore &operator=(Semaphore &&)      = delete;
  Semaphore &operator=(const Semaphore &) = delete;

  void post();

  void wait();

private:
  size_t                  m_res_limits_;
  std::mutex              m_sema_mutex_;
  std::condition_variable m_cond_;
};

#endif /* __SEMAPHORE__H__ */
