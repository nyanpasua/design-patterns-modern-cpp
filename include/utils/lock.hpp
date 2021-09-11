//
// Created by liyingmin on 2021/9/11.
//

#ifndef DESIGN_PATTERNS_LOCK_HPP
#define DESIGN_PATTERNS_LOCK_HPP

#include <atomic>

namespace okdp {
namespace utils {

/*!
@brief add lock via memory model

@since version 1.0.0
*/
class spin_lock {
 private:
  std::atomic_flag flag_ = ATOMIC_FLAG_INIT;

 public:
  void lock() {
    while (flag_.test_and_set(std::memory_order_acquire))
      ;
  }

  void unlock() { flag_.clear(std::memory_order_release); }
};

}  // namespace utils
}  // namespace okdp

#endif  // DESIGN_PATTERNS_LOCK_HPP
