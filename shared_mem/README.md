# Shared Memory IPC Demo

This project demonstrates Inter-Process Communication (IPC) between two processes using Windows Shared Memory and Mutexes.

## The Task
The goal is to have two instances of the same program synchronize and communicate:
1.  **Process 1 (Creator):** Creates a shared memory segment backed by a file (`gibrish.bin`) and waits for a signal.
2.  **Process 2 (Writer):** Connects to the existing shared memory and writes a specific character (`*`) to it.
3.  **Synchronization:** Use a named Mutex (`"printy"`) to ensure only one process creates the memory, avoiding race conditions where both might try to create it simultaneously.

## The Code
*   `SharedMem_summary.cpp`: The main C++ source code. It spawns a child process so two instances run simultaneously.
    *   Uses `CreateMutex` for synchronization.
    *   Uses `CreateFileMapping` / `OpenFileMapping` for shared memory.
    *   Uses `MapViewOfFile` to read/write data.

## Questions / Tips Needed
I'm looking for feedback on:
*   **Synchronization:** Is polling with `Sleep()` the best way for Process 1 to wait for Process 2? Would a named Event be better?
*   **Resource Management:** How can I better handle closing handles (RAII) to avoid leaks in C++?
*   **Error Handling:** Are there better ways to handle WinAPI errors?
