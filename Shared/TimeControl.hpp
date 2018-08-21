#pragma once
#include <chrono>
using microseconds = unsigned long long;
using Clock = std::chrono::high_resolution_clock;
using Point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Duration = std::chrono::duration<microseconds>;

inline microseconds now() {
	return std::chrono::duration_cast<std::chrono::microseconds>(Clock::now().time_since_epoch()).count();
}