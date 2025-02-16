#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>


DWORD WINAPI ThreadFunc(LPVOID param) {
    int id = (int)(uintptr_t)param;
    std::cout << "Поток " << id << " начал выполнение.\n";

    srand(static_cast<unsigned int>(time(nullptr)) + id);

    while (true) {
        int randomNumber = rand();
        std::cout << "Поток " << id << " сгенерировал число: " << randomNumber << "\n";

        Sleep(1000);
    }

    std::cout << "Поток " << id << " завершил выполнение.\n";
    return 0;
}

int main() {
    setlocale(LC_ALL, "Rus");

    std::vector<HANDLE> threads;
    int nextThreadId = 1;

    while (true) {
        std::cout << "1. Создать новый поток\n";
        std::cout << "2. Завершить поток\n";
        std::cout << "3. Выйти\n";
        std::cout << "Выберите действие: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)(uintptr_t)nextThreadId++, 0, NULL);
            if (hThread == NULL) {
                std::cerr << "Ошибка создания потока\n";
            }
            else {
                threads.push_back(hThread);
                std::cout << "Поток " << nextThreadId - 1 << " создан.\n";
            }
        }
        else if (choice == 2) {
            if (threads.empty()) {
                std::cout << "Нет активных потоков для завершения.\n";
            }
            else {
                std::cout << "Введите ID потока для завершения: ";
                int id;
                std::cin >> id;

                if (id > 0 && id <= threads.size()) {
                    TerminateThread(threads[id - 1], 0);
                    CloseHandle(threads[id - 1]);
                    threads.erase(threads.begin() + id - 1);
                    std::cout << "Поток " << id << " завершен.\n";
                }
                else {
                    std::cout << "Неверный ID потока.\n";
                }
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            std::cout << "Неверный выбор.\n";
        }
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        TerminateThread(threads[i], 0);
        CloseHandle(threads[i]);
    }

    std::cout << "Все потоки завершены. Нажмите любую клавишу для выхода.\n";
    _getch();

    return 0;
}