// Auto Clicker для Amazing Online для Лесоруба
#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

std::atomic<bool> running(false);
std::atomic<bool> shouldExit(false);

void SimulateKeyPress(WORD key) {
    INPUT input = { 0 };

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void AutoPress() {
    std::vector<WORD> keys = { 0x51, 0x57, 0x45, 0x52 }; // Q - 0x51, W - 0x57, E - 0x45, R - 0x52;

    while (!shouldExit) {
        if (running) {
            for (WORD key : keys) {
                SimulateKeyPress(key);

                int delay_between_keys = 5 + (rand() % 11);
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_between_keys));
            }

            int delay_between_cycles = 10 + (rand() % 21);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_between_cycles));
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

void CheckHotkey() {
    bool keyWasPressed = false;

    while (!shouldExit) {
        bool keyIsPressed = ((GetAsyncKeyState(VK_OEM_3) & 0x8000) != 0);

        if (keyIsPressed && !keyWasPressed) {
            running = !running;

            if (running) {
                std::cout << "Скрипт запущен (спам Q W E R)" << std::endl;
            }
            else {
                std::cout << "Скрипт остановлен" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        keyWasPressed = keyIsPressed;

        if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0) {
            shouldExit = true;
            std::cout << "Выход из программы..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "Авто-кликер Molten" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << "Нажмите ~ или Ё для запуска/остановки скрипта" << std::endl;
    std::cout << "Нажмите ESC для выхода из программы" << std::endl;
    std::cout << "Текущий статус: Остановлен" << std::endl;
    std::cout << "Скрипт будет нажимать: Q W E R" << std::endl;

    std::thread hotkeyThread(CheckHotkey);
    std::thread autoPressThread(AutoPress);

    hotkeyThread.join();
    autoPressThread.join();

    return 0;
}