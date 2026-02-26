#include <stdio.h>
#include <windows.h>

void childProc(int argc, char *argv[]) {}

int main(int argc, char *argv[]) {
    if (argc != 0) {
        childProc(argc, argv);
        return 0;
    }
    STARTUPINFO si[2];
    PROCESS_INFORMATION pi[2];
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(si));
    si[0].cb = sizeof(si[0]);
    si[1].cb = sizeof(si[1]);
    int success[2];
    for (int i = 0; i < 2; i++) {
        success[i] = CreateProcess(

        );
    }
    return 0;
}
