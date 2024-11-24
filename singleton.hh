#ifndef SINGLETON_HH
#define SINGLETON_HH

#include <functional>
#include <mutex>

namespace sys {
namespace utils {
template <typename CN> struct Del_instance {
  CN *instance{nullptr};
  std::function<void(const CN *)> dealloctor;

public:
  Del_instance() = default;
  Del_instance(std::function<void(const CN *)> dealloctor)
      : dealloctor(dealloctor) {}
  ~Del_instance() {
    if (instance) {
      if (dealloctor != nullptr) {
        dealloctor(instance);
      } else {
        delete instance;
      }
      instance = nullptr;
    }
  }
};

template <typename CN, typename... Args> CN *get_instance(Args&&... args) {
  static CN *_instance = nullptr;
  static std::mutex _mutex;
  static Del_instance<CN> _del;
  
  if (_instance == nullptr) {
    std::unique_lock<std::mutex> _lc(_mutex);
    if (_instance == nullptr) {
      _instance = new CN(std::forward(args...));
      _del.instance = _instance;
    }
  }
  return _instance;
}

template <typename CN, typename... Args>
CN *get_instance_custom_alloc(std::function<CN *(Args &&...)> allocator,
                              std::function<void(const CN *)> deallocator,
                              Args &&...args) {
  static CN *_instance = nullptr;
  static std::mutex _mutex;
  static Del_instance<CN> _del(deallocator);
  
  if (_instance == nullptr) {
    std::unique_lock<std::mutex> _lc(_mutex);
    if (_instance == nullptr) {
      _instance = allocator(std::forward(args...));
      _del.instance = _instance;
    }
  }
  return _instance;
}

} // namespace utils
} // namespace sys

#endif// SINGLETON_HH