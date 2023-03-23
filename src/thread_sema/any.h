#ifndef __ANY__H__
#define __ANY__H__

#include <iostream>
#include <memory>

class Any {
public:
  Any()  = default;
  ~Any() = default;

  template <typename T>
  Any(T data) : m_base_(std::make_unique<Derive<T>>(data)) {
  }

  template <typename T>
  T cast_() {
    Derive<T> *pb = dynamic_cast<Derive<T> *>(m_base_.get());
    if (pb == nullptr) {
      throw "type unmatch";
    }

    return pb->m_data_;
  }

  Any(Any &&)                 = default;
  Any &operator=(Any &&)      = default;
  Any(const Any &)            = delete;
  Any &operator=(const Any &) = delete;

private:
  class Base {
  public:
    virtual ~Base() = default;

  private:
  };

  template <class T>
  class Derive : public Base {
  public:
    Derive(T data) : m_data_(data) {
    }
    T m_data_;
  };

private:
  std::unique_ptr<Base> m_base_;
};

#endif /* __ANY__H__ */
