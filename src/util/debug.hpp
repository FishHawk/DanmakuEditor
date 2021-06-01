#include <fmt/core.h>

template <typename S, typename... Args>
inline void print(const S &format_str, Args &&...args) {
  fmt::print(format_str, args...);
}

template <typename S, typename... Args>
inline void println(const S &format_str, Args &&...args) {
  fmt::print(format_str, args...);
  std::putc('\n', stdout);
}