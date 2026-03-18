#include <windows.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#define PRIME_STORAGE 80000
#define SHARED_MEM_NAME "Global\\PrimeCounter"
#define GRANULARITY 10000

std::string getSelfPath() {
    char buffer[MAX_PATH];
    // Gets the full path of the current executable
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

typedef struct {
    int next_write_index;  // Where the next prime goes
    int starting_point;    // The next number to be checked
    DWORD primes[];        // The actual results
} SharedMemoryLayout;

typedef struct {
    DWORD end, total_cells, num_processes, num_threads;
} TaskDefinition;

struct Mapper {
    bool valid = false;
    HANDLE hMap = NULL;
    SharedMemoryLayout* pBuf = NULL;
    std::string name = SHARED_MEM_NAME;
    HANDLE hMutex = NULL;
};

Mapper GetMapper(DWORD total_cells) {
    Mapper m;
    while (m.hMap == NULL) {
        m.hMap = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            total_cells,
            SHARED_MEM_NAME);

        Sleep(10);
    }
    while (m.pBuf == NULL) {
        m.pBuf = (SharedMemoryLayout*)
            MapViewOfFile(m.hMap, FILE_MAP_ALL_ACCESS, 0, 0, total_cells);
        Sleep(10);  // cpu protection
    }

    while (m.hMutex == NULL) {
        m.hMutex = CreateMutexA(NULL, FALSE, "Global\\OptimutexPrime");
        Sleep(10);
    }

    m.valid = true;
    return m;
}

TaskDefinition GetTask(void) {
    TaskDefinition td;

    std::cout << "Insert prime find area: 0 -> ";
    std::cin >> td.end;
    td.total_cells =
        (DWORD)sizeof(DWORD) * ((80 / 72.0) * td.end / std::log(td.end));

    std::cout << "allocating " << td.total_cells << "int cells of shared memory"
              << std::endl;
    std::cout << "Now, how many processes do you want to use? ";
    std::cin >> td.num_processes;
    if (td.num_processes < 0) td.num_processes *= -1;
    if (td.num_processes == 0) td.num_processes = 1;
    std::cout << "Okay, then " << td.num_processes << " processes it is."
              << std::endl;
    std::cout << "How many threads per process? ";
    std::cin >> td.num_threads;
    if (td.num_threads < 0) td.num_threads *= -1;
    if (td.num_threads == 0) td.num_threads = 1;
    std::cout << "Okay, then " << td.num_threads << " threads it is."
              << std::endl;
    return td;
}

class CalcProc {
   public:
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    DWORD total_cells;

    CalcProc(DWORD total_cells, DWORD num_threads, DWORD end) {
        // TODO:logic to open process with theses passed params.
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        bool failure = true;
        std::stringstream ss;
        std::string self_path = getSelfPath();
        std::string cmdLine;

        while (failure) {
            std::stringstream ss;
            ss << "\"" << self_path << "\" --worker " << total_cells << " "
               << num_threads << " " << end;
            cmdLine = ss.str();
            failure = !CreateProcessA(
                NULL,
                cmdLine.data(),
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi);
            Sleep(10);  // CPU protection.
        }
        this->total_cells = total_cells;
    }

    ~CalcProc() {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
};

bool isPrime(DWORD n) {
    if (n < 2) return false;
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i++;
    }
    return true;
}

void childProc(int argc, char* argv[]);

void worker_thread(DWORD end, DWORD total_cells, Mapper* shared_mem);

int main(int argc, char* argv[]) {
    // ── process sorting ─────────────────────────────────────────────────
    if (argc > 1) {
        childProc(argc, argv);
        return 0;
    }
    // ── task definision ─────────────────────────────────────────────────
    const TaskDefinition td = GetTask();
    // ── Creating shared memory ─────────────────────────────────────────────────
    Mapper shared_memory = GetMapper(td.total_cells);

    ZeroMemory(
        (void*)shared_memory.pBuf->primes,
        td.total_cells - 2 * sizeof(int));

    shared_memory.pBuf->next_write_index = 0;
    shared_memory.pBuf->starting_point = 1;

    auto begin = std::chrono::steady_clock::now();
    // ── spawning child processes──────────────────────
    std::vector<CalcProc> children;
    children.reserve(td.num_processes);
    for (int i = 0; i < td.num_processes; i++) {
        children.emplace_back(td.total_cells, td.num_threads, td.end);
    }
    // ── manage the processes ─────────────────────────────────────────────────
    while (shared_memory.pBuf->starting_point <= td.end) {
        double progress =
            (double)shared_memory.pBuf->starting_point / td.end * 100.0;
        if (progress > 100) progress = 100;
        std::cout << "\rProgress: " << (int)progress << "%    " << std::flush;
        Sleep(100);
    }

    children.clear();

    auto end = std::chrono::steady_clock::now();
    std::cout << "This evaluation took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end - begin)
                         .count() /
                     1000.0
              << "s" << std::endl;
    // ── making it pretty ─────────────────────────────────────────────────
    std::sort(
        shared_memory.pBuf->primes,
        &shared_memory.pBuf->primes[shared_memory.pBuf->next_write_index]);
    std::cout << "10 first primes = [" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "    " << shared_memory.pBuf->primes[i] << ",\n";
    }
    std::cout << "]" << std::endl;
    return 0;
}

void childProc(int argc, char* argv[]) {
    // ── init ─────────────────────────────────────────────────
    DWORD total_cells = std::stoul(argv[2]);
    DWORD num_threads = std::stoul(argv[3]);
    DWORD end = std::stoul(argv[4]);
    Mapper shared_mem = GetMapper(total_cells);
    // ── manage threads ─────────────────────────────────────────────────
    std::vector<std::thread> workers;
    for (int i = 0; i < num_threads; i++) {
        workers.emplace_back(worker_thread, end, total_cells, &shared_mem);
    }
    for (int i = 0; i < num_threads; i++) {
        workers[i].join();
    }
    CloseHandle(shared_mem.hMap);
    CloseHandle(shared_mem.hMutex);
}

void worker_thread(DWORD end, DWORD total_cells, Mapper* shared_mem) {
    while (true) {
        WaitForSingleObject(shared_mem->hMutex, INFINITE);
        DWORD start = shared_mem->pBuf->starting_point;
        shared_mem->pBuf->starting_point += GRANULARITY;
        DWORD area_end = (std::min)(start + GRANULARITY, end);
        ReleaseMutex(shared_mem->hMutex);
        if (start > end) break;
        std::vector<DWORD> primes;
        // ── find primes ─────────────────────────────────────────────────
        for (DWORD i = start; i < area_end; i++) {
            if (isPrime(i)) primes.push_back(i);
        }

        WaitForSingleObject(shared_mem->hMutex, INFINITE);
        int i;
        for (i = 0; i < primes.size(); i++) {
            shared_mem->pBuf->primes[shared_mem->pBuf->next_write_index + i] =
                primes[i];
        }
        shared_mem->pBuf->next_write_index += primes.size();
        ReleaseMutex(shared_mem->hMutex);
    }
}