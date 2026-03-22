#include <windows.h>

#include <string>
#include <vector>

struct PipeData {
    short name_length;
    char payload[];
};

class Pipe {
   private:
    std::vector<char> buffer;
    HANDLE hMap = NULL;
    HANDLE hMutex = NULL;
    PipeData* data = NULL;

   public:
    std::string name;

    Pipe(std::string name) {
        this->name = name;
        // ── create a mutex if not exists──────────────────
        std::string mutex_name = "__" + name + "__";
        while (hMutex == NULL) {
            hMutex = CreateMutexA(NULL, FALSE, mutex_name.data());
            Sleep(10);
        }
        // ── create shared memory if not exists────────────

        while (hMap == NULL) {
            hMap = CreateFileMappingA(
                INVALID_HANDLE_VALUE,
                NULL,
                PAGE_READWRITE,
                0,
                200,
                name.data());
        }

        while (data == NULL) {
            data = (PipeData*)MapViewOfFile(hMap, NULL, 0, 200, 200);
        }
    }

    std::string read() {
        WaitForSingleObject(hMutex, INFINITE);
        std::string pipe_buffer = data->payload;
        if () ReleaseMutex(hMutex);
    }
}