#include <windows.h>
#include <conio.h>
#include <iostream>



DWORD WINAPI Input_func(LPVOID param)
{
    int id = (int)(uintptr_t)param;
    std::cout << " Отгадайте число загаданное  ";
}

DWORD WINAPI Output_func(LPVOID param) 
{

}

DWORD WINAPI Random_func()
{

}

int main() {

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    HANDLE threads[5];

    int quantity_threads;
    std::cout << "Введите количество потоков (от 2 до 5): ";
    std::cin >> quantity_threads;

    if (quantity_threads < 2 || quantity_threads > 5) {
        std::cerr << "Неверное число черепах! Введите от 5 до 10.\n";
        return 1;
    }

    for (int i = 0; i < quantity_threads; i++) {
        threads[i] = CreateThread(NULL, 0, Input_func, (LPVOID)(uintptr_t)(i + 1), 0, NULL);
        if (threads[i] == NULL) {
            std::cerr << "Ошибка создания потока " << i + 1 << "\n";
            return GetLastError();
        }
    }

    wchar_t cmdline[256];
    swprintf_s(cmdline, L"C:\\Users\\st310-11\\Desktop\\меньшиков ПР-32\\LabProcess-main\\LabProcess12child\\x64\\Debug\\LabProcess12child.exe %d", (int)threads);

    if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "Ошибка создания процесса: " << GetLastError() << std::endl;
        CloseHandle(threads);
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);

    std::cout << "Дочерний процесс завершен. Нажмите любую клавишу для выхода.\n";
    _getch();

    CloseHandle(threads);
    return 0;
}