#ifndef __THREAD__H__
#define __THREAD__H__

#include <functional>

class Thread {
public:
  using threadFunc = std::function<void(std::size_t)>;

  Thread(threadFunc func);

  ~Thread();

  Thread(Thread &&)                 = delete;
  Thread(const Thread &)            = delete;
  Thread &operator=(const Thread &) = delete;
  Thread &operator=(Thread &&)      = delete;

  void start();

  std::size_t getThreadId() const;

private:
  static std::size_t m_gen_id_;
  threadFunc         m_thread_func_;
  std::size_t        m_thread_id_;
};

#endif /* __THREAD__H__ */
