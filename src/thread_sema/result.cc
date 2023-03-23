#include "result.h"

Result::Result(std::shared_ptr<Task> sp, bool flag)
    : m_task_(sp), m_isvalid_(flag) {
  sp->setResult(this);
}

Result::~Result() {
}

Any Result::get() {
  if (!m_isvalid_) {
    return "";
  }
  /// 任务未执行完成，阻塞等待
  m_sema_.wait();
  return std::move(m_any_);
}

void Result::setVal(Any any) {
  m_any_ = std::move(any);
  m_sema_.post();
}
