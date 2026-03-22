#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <primes.hpp>

static constexpr uint32_t MAX_PATH{500};
static constexpr size_t ARG_COUNT{3};

int main(int argc, char* argv[]) {
    assert(argc == ARG_COUNT);
    std::uint32_t thrd_count(std::atoi(argv[1]));
    std::uint32_t end(std::atoi(argv[2]));

    findprms(thrd_count, end);
    return 0;
}
