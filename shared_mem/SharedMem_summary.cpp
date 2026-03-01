#include <stdio.h>
#include <string.h>
#include <windows.h>

#define FILE_PATH "..\\gibrish.bin"

void childProc();

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "childProc") == 0) {
        childProc();
        return 0;
    }

    STARTUPINFO si[2];
    PROCESS_INFORMATION pi[2];
    ZeroMemory(si, sizeof(si));
    ZeroMemory(pi, sizeof(pi));
    si[0].cb = sizeof(si[0]);
    si[1].cb = sizeof(si[1]);
    BOOL success[2] = {TRUE, TRUE};

    char selfPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, selfPath, MAX_PATH) == 0) {
        printf("Error: GetModuleFileName failed.\n");
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
            printf("process %d failed", i);
            continue;
        }
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }
    return 0;
}

void childProc() { printf("debug\n"); }