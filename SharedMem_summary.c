#include <windows.h>
#include <stdio.h>
int main(int argc, char *argv[]){
  if(argc!=0){
    childProc(argc, argv);
    return 0;
  }
  STARTUPINFO si[];
  PROCESSINFO pi[];
  ZeroMemory(&pi);
  ZeroMemory(&si);
  
  return 0;
}
