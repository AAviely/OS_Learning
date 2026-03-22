#include <stdio.h>
#include <windows.h>

#include <iostream>

#define FILE_PATH "..\\gibrish.bin"

void childProc();

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "childProc") == 0) {
        childProc();
        return 0;
    }
    HANDLE hMainMutex = CreateMutexA(NULL, FALSE, "printy");

    STARTUPINFO si[2];
    PROCESS_INFORMATION pi[2];
    ZeroMemory(si, sizeof(si));
    ZeroMemory(pi, sizeof(pi));
    si[0].cb = sizeof(si[0]);
    si[1].cb = sizeof(si[1]);
    BOOL success[2] = {TRUE, TRUE};

    char selfPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, selfPath, MAX_PATH) == 0) {
        std::cout << "Error: GetModuleFileName failed." << std::endl;
        return 1;
    }
    char cmd[MAX_PATH + 32];

    for (int i = 0; i < 2; i++) {
        sprintf_s(cmd, sizeof(cmd), "\"%s\" %s", selfPath, "childProc");
        success[i] =
            CreateProcessA(NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
                           NULL, NULL, &si[i], &pi[i]);
    }

    for (int i = 0; i < 2; i++) {
        if (!success[i]) {
            std::cout << "process " << i << " failed" << std::endl;
            continue;
        }
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }
    if (hMainMutex) CloseHandle(hMainMutex);
    return 0;
}

void childProc() {
    HANDLE hMutex = CreateMutexA(NULL, FALSE, "printy");
    // CreateMutex returns NULL (0) on failure.
    if (hMutex == NULL) return;

    WaitForSingleObject(hMutex, INFINITE);
    HANDLE hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "myMap");
    if (hMap == NULL) {
        HANDLE hFile;
        // CAUTION: CreateFile returns INVALID_HANDLE_VALUE (-1) on failure, NOT
        // NULL.
        hFile = CreateFileA(FILE_PATH, GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            std::cout << "Couldn't open this gibrish.bin. See error "
                      << GetLastError() << std::endl;
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
            return;
        }
        hMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 1, "myMap");
        // CreateFileMapping returns NULL (0) on failure.
        if (hMap == NULL) {
            printf("Mapping gibrish.bin failed with error code %d",
                   (int)GetLastError());
            ReleaseMutex(hMutex);
        } else {
            // Process 1 must stay alive so the mapping object remains valid for
            // Process 2
            ReleaseMutex(hMutex);
            LPVOID a = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 1);
            // MapViewOfFile returns NULL (0) on failure.
            if (a == NULL) {
                std::cout << "Process 1 failed to map view. Error: "
                          << GetLastError() << std::endl;
            } else {
                while (*((volatile char*)a) != '*') {
                    Sleep(100);  // Poll every 100ms
                }
                UnmapViewOfFile(a);
            }
            CloseHandle(hMap);
        }
        CloseHandle(hFile);
        CloseHandle(hMutex);
    } else {
        ReleaseMutex(hMutex);
        LPVOID a = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 1);
        if (a == NULL) {
            std::cout << "failed to map gibrish.bin, error " << GetLastError()
                      << std::endl;
            CloseHandle(hMutex);
            CloseHandle(hMap);
            return;
        } else {
            *((char*)a) = '*';
            BOOL b = UnmapViewOfFile(a);

            if (!b) {
                std::cout << "couldn't unmap gibrish.bin (error "
                          << GetLastError() << ')' << std::endl;
                CloseHandle(hMutex);
                CloseHandle(hMap);
                return;
            }
        }
        CloseHandle(hMutex);
        CloseHandle(hMap);
        std::cout << "succeeded in operation!" << std::endl;
    }
}