#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[]) {
  if (argc != 0) {
    childProc(argc, argv);
    return 0;
  }
  STARTUPINFO si[2];
  PROCESS_INFORMATION pi[2];
  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&si);
  for (int i = 0; i < 2; i++) {
    CreateProcess(

    );
  }
  return 0;
}
