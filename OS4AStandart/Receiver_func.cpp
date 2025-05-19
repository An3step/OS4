#include <iostream>
#include <limits>
#include <string>
#include "Receiver_func.h"

void Input_Value(int& value, int lowerBound)
{
    while (true)
    {
        std::cout << "Введите значение (не ниже " << lowerBound << "): ";
        std::cin >> value;
        if (std::cin.fail() || value < lowerBound)
        {
            std::cin.clear(); // Сброс состояния потока
            std::cin.ignore(100000, '\n'); // Очистка входного буфера
            std::cout << "Некорректный ввод. Попробуйте снова.\n";
        }
        else
        {
            std::cin.ignore(100000, '\n'); // Очистка входного буфера
            break; // Выход из цикла, если ввод корректен
        }
    }
}

void Input_Value(std::string& value, int lowerBound)
{
    while (true)
    {
        std::cout << "Введите значение (не ниже " << lowerBound << "): ";
        std::cin >> value;
        if (std::cin.fail() || value.length() < lowerBound)
        {
            std::cin.clear(); // Сброс состояния потока
            std::cin.ignore(100000, '\n'); // Очистка входного буфера
            std::cout << "Некорректный ввод. Попробуйте снова.\n";
        }
        else
        {
            std::cin.ignore(100000, '\n'); // Очистка входного буфера
            break; // Выход из цикла, если ввод корректен
        }
    }
}

void CreateEvents(HANDLE& hControlEvent, std::vector<HANDLE>& events, int process_count)
{
    hControlEvent = CreateEventA(NULL, TRUE, FALSE, "Global\\ControlEvent");
    if (hControlEvent == NULL)
    {
        std::cerr << "Ошибка создания контрольного события: " << GetLastError() << std::endl;
        CloseHandle(hControlEvent);
        return;
    }

    for (int i = 0; i < process_count; ++i)
    {
        std::string event_name = "Global\\MyEvent" + std::to_string(i);
        HANDLE hEvent = NULL;
        for (int attempt = 0; attempt < 5; ++attempt) {
            hEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, event_name.c_str());
            if (hEvent != NULL) break;
            Sleep(100); // Wait a bit before retrying
        }

        if (hEvent == NULL)
        {
            std::cerr << "Не удалось открыть событие: " << GetLastError() << std::endl;
            continue;
        }
        events.push_back(hEvent);
    }
}

void CreateSenderProcess(const std::string& filename, int index, int max_records)
{
    std::string command = "Sender/Sender.exe " + filename + " " + std::to_string(index) + " " + std::to_string(max_records);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Не удалось создать процесс: " << GetLastError() << std::endl;
        return;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void CreateBinaryFile(const std::string& filename)
{
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile)
    {
        std::cerr << "Не удалось создать двоичный файл!" << std::endl;
        return;
    }
    outFile.close();
}

void ReadBinaryFile(const std::string& filename)
{
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile)
    {
        std::cerr << "Error in opening file\n";
        return;
    }
    std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    if (content.size() < 20)
    {
        std::cerr << "File is containing less than 1 record.\n";
        return;
    }
    std::cout << "Message: " << content.substr(0, 20) << "\n";
    std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
    if (!outFile)
    {
        std::cerr << "Error in opening file for writing\n";
        return;
    }
    outFile << content.substr(20);
    outFile.close();
}