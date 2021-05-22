#pragma once

#include <chrono>

namespace util {

class Timer {
public:
  Timer() { restart(); };

  void restart() { _start = std::chrono::system_clock::now(); };
  double elapsed() {
    return std::chrono::duration<double>(
               std::chrono::system_clock::now() - _start)
        .count();
  };

private:
  std::chrono::time_point<std::chrono::system_clock> _start;
};

} // namespace util