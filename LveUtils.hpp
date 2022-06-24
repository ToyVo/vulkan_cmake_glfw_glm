#ifndef LVE__LVEUTILS_HPP_
#define LVE__LVEUTILS_HPP_

#include <functional>

namespace lve {
  template<typename T, typename... Rest>
    void hashCombine(std::size_t &seed, const T &v, const Rest &...rest) {
      seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      (hashCombine(seed, rest), ...);
    }
}// namespace lve

#endif//LVE__LVEUTILS_HPP_
