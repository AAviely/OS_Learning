#pragma once

#include <cstdint>
#include <deque>

bool isPrime(std::uint32_t n) noexcept;
std::deque<std::uint32_t> findprms(std::uint32_t thrd_count, std::uint32_t end) noexcept;
