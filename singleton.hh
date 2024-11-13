#ifndef SINGLETON_HH
#define SINGLETON_HH

#include <mutex>

namespace sys {
namespace utils {
template <typename CN> struct Del_singleton {
  CN *instance{nullptr};
  ~Del_singleton() {
    if (instance) {
      delete instance;
      instance = nullptr;
    }
  }
};

template <typename CN, typename... Args> CN *get_singleton_instance(Args&&... args) {
  static CN *_instance = nullptr;
  static std::mutex _mutex;
  static Del_singleton<CN> _del;
  
  if (_instance == nullptr) {
    std::unique_lock<std::mutex> _lc(_mutex);
    if (_instance == nullptr) {
      _instance = new CN(std::forward(args...));
      _del.instance = _instance;
    }
  }
  return _instance;
}
} // namespace utils
} // namespace sys

#endif// SINGLETON_HH