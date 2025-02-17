#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>

DWORD WINAPI ThreadFunc(LPVOID param) {
    int id = (int)(uintptr_t)param;
    std::cout << "Поток " << id << " начал выполнение.\n";

    srand(static_cast<unsigned int>(time(nullptr)) + id);

    while (true) {
        int randomNumber = rand();
        std::cout << "Поток " << id << " сгенерировал число: " << randomNumber << "\n";
        Sleep(1000);
    }

    return 0;
}

int main() {

    const wchar_t* pipeName = L"\\\\.\\pipe\\MyPipe";

    HANDLE hPipe = CreateFile(pipeName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка подключения к каналу: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Подключение к каналу установлено.\n";

    std::vector<HANDLE> threads;
    int nextThreadId = 1;

    while (true) {
        wchar_t command[100];
        DWORD bytesRead;

        if (ReadFile(hPipe, command, sizeof(command), &bytesRead, NULL)) {
            if (wcscmp(command, L"1") == 0) {

                HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)(uintptr_t)nextThreadId++, 0, NULL);
                if (hThread == NULL) {
                    std::cerr << "Ошибка создания потока\n";
                }
                else {
                    threads.push_back(hThread);
                    std::cout << "Поток " << nextThreadId - 1 << " создан.\n";
                }
            }
            else if (wcscmp(command, L"2") == 0) {
                break;
            }
        }
        else {
            std::cerr << "Ошибка чтения из канала: " << GetLastError() << std::endl;
            break;
        }
    }

    for (HANDLE hThread : threads) {
        TerminateThread(hThread, 0);
        CloseHandle(hThread);
    }

    CloseHandle(hPipe);
    std::cout << "Дочерний процесс завершен.\n";
    return 0;
}