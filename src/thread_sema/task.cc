#include "task.h"

Task::Task() : m_result_(nullptr) {
}

Task::~Task() {
}

void Task::exec() {
  if (m_result_ != nullptr) {
    Any any = run();
    m_result_->setVal(std::move(any));
  }
}

void Task::setResult(Result *res) {
  m_result_ = res;
}
