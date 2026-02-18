#include <windows.h>
#include <stdio.h>
int main(int argc, char *argv[]){
  if(argc!=0){
    childProc(argc, argv);
    return 0;
  }
  STARTUPINFO si[2];
  PROCESSINFO pi[2];
  ZeroMemory(&pi);
  ZeroMemory(&si);
  for(int i = 0; i < 2; i++){
      CreateProcess(
              
        );
  }
  return 0;
}
