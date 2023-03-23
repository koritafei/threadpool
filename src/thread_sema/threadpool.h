#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>

#include "result.h"
#include "task.h"
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
  /// @param sp
  Result submitTask(std::shared_ptr<Task> sp);
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
  std::queue<std::shared_ptr<Task>> m_tasks_;
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
