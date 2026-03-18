#include <windows.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#define PRIME_STORAGE 80000

#define PATH                                         \
    "C:\\Users\\USER\\OneDrive\\מסמכים\\GitHub\\OS_" \
    "Learning\\ParallelPrimeSearch\\build\\ParallelPrimeSearch.exe"

std::string getSelfPath() {
    char buffer[MAX_PATH];
    // Gets the full path of the current executable
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

class CalcProc {
   public:
    HANDLE hProc;
    LPVOID area_start;
    DWORD area_length;

    CalcProc(LPVOID area_start, DWORD area_length) {
        // TODO:logic to open process with theses passed params.
        bool failure = true;
        std::stringstream ss;
        ss << PATH << " --worker " << area_start << " " << area_length;
        std::string cmdLine = ss.str();
        std::cout << cmdLine << std::endl;

        // while (failure) {
        //     failure = !CreateProcessA(
        //         NULL,

        //     )
        // }
        this->area_length = area_length;
        this->area_start = area_start;
    }

    ~CalcProc() { std::cout << "now deleted me" << std::endl; }
};

void childProc(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    if (argc > 1) {
        childProc(argc, argv);
        return 0;
    }
    std::vector<CalcProc*> children;
    for (int i = 0; i < 10; i++) {
        children.push_back(new CalcProc((LPVOID)15, 10));
    }
    for (int i = 0; i < 10; i++) {
        CalcProc* proc = children.back();
        children.pop_back();
        delete proc;
    }
    // main logic
    return 0;
}

void childProc(int argc, char* argv[]) {}