#include "threadpool.h"

/// @brief任务队列最大值
const size_t TASK_THRESHOLD = 1024;
/// @brief 线程池线程最大值
const size_t  THREAD_THRESHOLD     = 20;
const int32_t THREAD_IDLE_MAX_TIME = 60;

ThreadPool::ThreadPool()
    : m_init_threads_size_(4),
      m_thread_threshold_(THREAD_THRESHOLD),
      m_threads_size_(0),
      m_idle_thread_size_(0),
      m_task_size_(0),
      m_task_threshold_(TASK_THRESHOLD),
      m_is_running_(false) {
}

ThreadPool::~ThreadPool() {
  m_is_running_ = false;
  std::unique_lock<std::mutex> lock{m_task_mutex_};
  m_task_not_empty_.notify_all();
  m_exit_pool_.wait(lock, [&]() -> bool {
    return m_threads_size_ == 0;
  });
}

bool ThreadPool::checkThreadPoolRuning() {
  return m_is_running_;
}

void ThreadPool::setTaskThresHold(size_t taskthreshold) {
  if (checkThreadPoolRuning()) {
    return;
  }
  m_task_threshold_ = taskthreshold;
}

void ThreadPool::setThreadPoolMode(ThreadPoolMode mode) {
  if (checkThreadPoolRuning()) {
    return;
  }
  m_threadpool_mode_ = mode;
}

void ThreadPool::setThreadThresHold(size_t thread_threshold) {
  if (checkThreadPoolRuning()) {
    return;
  }
  m_thread_threshold_ = thread_threshold;
}

void ThreadPool::start(int threadsize) {
  m_init_threads_size_ = threadsize;
  m_idle_thread_size_  = threadsize;
  m_threads_size_      = threadsize;

  for (size_t i = 0; i < m_init_threads_size_; i++) {
    auto ptr = std::make_unique<Thread>(
        std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
    auto threadid = ptr->getThreadId();
    std::cout << "create thread: " << threadid << std::endl;
    m_threads_.emplace(threadid, std::move(ptr));
  }

  for (size_t i = 0; i < m_init_threads_size_; i++) {
    m_threads_[i]->start();
  }
}

Result ThreadPool::submitTask(std::shared_ptr<Task> sp) {
  std::unique_lock<std::mutex> lock{m_task_mutex_};
  if (!m_task_not_full_.wait_for(lock, std::chrono::seconds(1), [&]() -> bool {
        return m_task_size_ < m_task_threshold_;
      })) {
    std::cerr << "submit task error" << std::endl;
    return Result{sp, false};
  }
  m_task_size_++;
  m_tasks_.emplace(sp);
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

  return Result{sp, true};
}

void ThreadPool::threadFunc(std::size_t threadid) {
  auto lasttime = std::chrono::high_resolution_clock().now();
  for (;;) {
    std::shared_ptr<Task> task;
    {
      std::unique_lock<std::mutex> lock{m_task_mutex_};

      while (m_task_size_ == 0) {
        if (!m_is_running_) {
          //回收线程
          std::cout << "delete thread: " << threadid << std::endl;

          m_threads_.erase(threadid);
          m_threads_size_--;
          m_idle_thread_size_--;
          m_exit_pool_.notify_all();
          return;
        }

        if (ThreadPoolMode::THREAD_MODE_CACHED == m_threadpool_mode_) {
          if (std::cv_status::timeout ==
              m_task_not_empty_.wait_for(lock, std::chrono::seconds(1))) {
            auto now  = std::chrono::high_resolution_clock().now();
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(
                now - lasttime);
            if (diff.count() >= THREAD_IDLE_MAX_TIME &&
                m_threads_size_ > m_init_threads_size_) {
              //回收线程
              std::cout << "delete thread: " << threadid << std::endl;
              m_threads_.erase(threadid);
              m_threads_size_--;
              m_idle_thread_size_--;
              m_exit_pool_.notify_all();
              return;
            }
          }
        } else {
          m_task_not_empty_.wait(lock);
        }
      }

      task = m_tasks_.front();
      m_tasks_.pop();
      m_task_size_--;
      m_idle_thread_size_--;
      if (m_task_size_ > 0) {
        m_task_not_empty_.notify_all();
      }
      m_task_not_full_.notify_all();
    }

    if (task != nullptr) {
      task->exec();
      m_idle_thread_size_++;
    }

    lasttime = std::chrono::high_resolution_clock().now();
  }
}