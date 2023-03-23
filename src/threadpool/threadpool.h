#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>

#include "thread.h"

/// @brief 线程池模式
enum class ThreadPoolMode {
  THREAD_MODE_FIXED,   // 固定数据量
  THREAD_MODE_CACHED,  // 动态线程数量
};

class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();

  ThreadPool(const ThreadPool &)            = delete;
  ThreadPool(ThreadPool &&)                 = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&)      = delete;

  /// @brief 线程池启动
  void start(int threadssize = 4);
  /// @brief 提交任务
  /// @tparam Func
  /// @tparam ...Args
  /// @param func
  /// @param ...args
  /// @return
  template <typename Func, typename... Args>
  auto submitTask(Func &&func, Args &&...args)
      -> std::future<decltype(func(args...))>;
  /// @brief 设置最大任务数
  /// @param task_threshold
  void setTaskThresHold(size_t task_threshold);

  /// @brief 设置最大线程数
  /// @param
  void setThreadThresHold(size_t thread_threshold);

  /// @brief 设置线程mode
  /// @param mode
  void setThreadPoolMode(ThreadPoolMode mode);

private:
  /// @brief 线程函数
  void threadFunc(std::size_t threadid);

  /// 检查线程池是否运行
  bool checkThreadPoolRuning();

private:
  /// @brief 线程数组
  std::unordered_map<std::size_t, std::unique_ptr<Thread>> m_threads_;
  /// @brief 线程池大小
  std::size_t m_init_threads_size_;
  /// 线程池最大大小
  std::size_t m_thread_threshold_;
  /// 线程池总线程数量大小
  std::atomic_uint32_t m_threads_size_;
  /// 线程池空闲线程数量大小
  std::atomic_uint32_t m_idle_thread_size_;
  /// @brief 线程任务队列
  using Task = std::function<void()>;
  std::queue<Task> m_tasks_;
  /// @brief 任务队列个数
  std::atomic_uint32_t m_task_size_;
  /// @brief 最大线程任务个数
  std::size_t m_task_threshold_;
  /// @brief 任务队列互斥锁
  std::mutex m_task_mutex_;
  /// @brief 任务队列未满信号量
  std::condition_variable m_task_not_full_;
  /// @brief 任务队列费控信号量
  std::condition_variable m_task_not_empty_;
  /// @brief 线程池模式
  ThreadPoolMode m_threadpool_mode_;
  /// 线程池运行标志
  std::atomic_bool m_is_running_;
  /// 线程池退出信号量
  std::condition_variable m_exit_pool_;
};

template <typename Func, typename... Args>
auto ThreadPool::submitTask(Func &&func, Args &&...args)
    -> std::future<decltype(func(args...))> {
  using RType = decltype(func(args...));

  auto task = std::make_shared<std::packaged_task<RType()>>(
      std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

  std::future<RType> result = task->get_future();

  std::unique_lock<std::mutex> lock{m_task_mutex_};
  if (!m_task_not_full_.wait_for(lock, std::chrono::seconds(1), [&]() -> bool {
        return m_task_size_ < m_task_threshold_;
      })) {
    std::cerr << "submit task error" << std::endl;
    auto task = std::make_shared<std::packaged_task<RType()>>([]() -> RType {
      return RType();
    });
    (*task)();
    return task->get_future();
  }
  m_task_size_++;
  m_tasks_.emplace([task]() {
    (*task)();
  });
  m_task_not_empty_.notify_all();

  // 动态创建线程
  if (ThreadPoolMode::THREAD_MODE_CACHED == m_threadpool_mode_ &&
      m_task_size_ > m_idle_thread_size_ &&
      m_threads_size_ < m_thread_threshold_) {
    auto ptr = std::make_unique<Thread>(
        std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
    auto threadid = ptr->getThreadId();
    std::cout << "create thread: " << threadid << std::endl;

    m_threads_.emplace(threadid, std::move(ptr));
    m_threads_[threadid]->start();
    m_idle_thread_size_++;
    m_threads_size_++;
  }

  return result;
}
