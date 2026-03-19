#include <chrono>
#include <cstdint>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <primes.hpp>

static constexpr uint32_t MAX_PATH{500};
static constexpr uint32_t WINDOW_SIZE{16384};

bool isPrime(std::uint32_t n) noexcept {
    if (n < 2) return false;
    std::uint32_t i(2);
    while (i * i <= n)
        if (!n % i++) return false;
    return true;
}

std::deque<std::uint32_t> findprms(std::uint32_t thrd_count, std::uint32_t end_num) noexcept {
    std::cout << "[" << thrd_count << ", " << end_num << "]\n";
    std::uint32_t cur_num(2);
    bool done(false);

    std::mutex mut;
    std::deque<std::future<std::deque<uint32_t>>> v_futures;
    std::deque<std::uint32_t> res;

    auto get_new_window = [&mut, &cur_num, &done, &end_num]() -> std::uint32_t {
        std::lock_guard lk(mut);
        auto res = cur_num;
        cur_num += WINDOW_SIZE;
        if ((cur_num += WINDOW_SIZE) > end_num) done = true;
        return res;
    };

    auto worker = [&get_new_window, &done]() -> std::deque<uint32_t> {
        std::deque<uint32_t> found;
        for (std::uint32_t cur(get_new_window()), cur_end(cur + WINDOW_SIZE);; ++cur) {
            if (cur == cur_end) {
                if (done) return found;
                cur     = get_new_window();
                cur_end = cur + WINDOW_SIZE;
            }
            if (isPrime(cur)) found.emplace_back(cur);
        }

        std::cout << "badaba\n";
        return found;
    };

    auto begin = std::chrono::steady_clock::now();

    for (std::uint32_t i(0); i < thrd_count; ++i)
        v_futures.push_back(std::async(std::launch::async, worker));

    for (auto& fut : v_futures)
        for (std::uint32_t& val : fut.get()) res.push_back(val);

    auto end = std::chrono::steady_clock::now();
    // std::sort(res.begin(), res.end());
    std::cout
        << "This evaluation took "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
        / 1000.0
        << "s\n";
    return res;
}
