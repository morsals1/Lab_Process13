#include <windows.h>
#include <conio.h>
#include <iostream>

int main() {

    setlocale(LC_ALL, "Rus");

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    wchar_t cmdline[] = L"C:\\Users\\mensh\\Desktop\\Lab_Process13child\\Lab13Child\\x64\\Debug\\Lab13Child.exe";

    if (!CreateProcess(
        NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Ошибка создания дочернего процесса: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "Дочерний процесс завершен. Нажмите любую клавишу для выхода.\n";
    _getch();

    return 0;
}