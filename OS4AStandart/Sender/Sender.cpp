#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

const int MAX_LENGTH = 20;

// int OpenMutexes()
// {
// }

int main(int argc, char* argv[]) {
    if (argc != 4)
    {
        std::cerr << "Usage: Sender.exe <filename> <index> <max_records>" << std::endl;
        return 1;
    }
    cout << "Process\n";
    std::string filename = std::string(argv[1]);
    std::string event_name = "Global\\MyEvent" + std::string(argv[2]);
    int max_record = std::stoi(argv[3]);

    HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, event_name.c_str());
    if (hEvent == NULL)
    {
        std::cerr << "Error creating the event: " << GetLastError() << std::endl;
        return 1;
    }
    cout << "Process?\n";
    SetEvent(hEvent);
    std::cout << "Event signalling of being ready." << std::endl;
    HANDLE WaitEvents[2];

    WaitEvents[1] = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Global\\MyGlobalMutex");
    if (WaitEvents[1] == NULL) {
        std::cerr << "Error opening the mutex: " << GetLastError() << std::endl;
        CloseHandle(hEvent);
        CloseHandle(WaitEvents[1]);
        return 1;
    }
    WaitForSingleObject(WaitEvents[1], INFINITE);
    ReleaseMutex(WaitEvents[1]);
    WaitEvents[0] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "Global\\ControlEvent");
    if (WaitEvents[0] == NULL) {
        std::cerr << "Error opening control event: " << GetLastError() << std::endl;
        CloseHandle(hEvent);
        return 1;
    }
    while(true)
    {
        DWORD WaitResult = WaitForMultipleObjects(2, WaitEvents, FALSE, INFINITE);
        if(WaitResult == WAIT_OBJECT_0)
        {
            cout << "Got signal on terminating work\n";
            break;
        }
        std::ifstream inputFile(filename, std::ios::binary | std::ios::ate);
        if (!inputFile)
        {
            std::cerr << "Error opening file for reading: " << GetLastError() << std::endl;
            ReleaseMutex(WaitEvents[1]);
            continue;
        }

        std::streamsize size = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);
        inputFile.close();

        if (size < 20 * max_record)
        {
            std::string message;
            std::cout << "Enter message(less than 20 characters): ";
            std::getline(std::cin, message);
            if (message.empty()) {
                std::getline(std::cin, message); // Try again if first attempt failed
            }
            if (message.length() > MAX_LENGTH) {
                message = message.substr(0, MAX_LENGTH);
            }
            message.resize(MAX_LENGTH, ' ');

            std::ofstream outputFile(filename, std::ios::binary | std::ios::app);
            if (!outputFile) {
                std::cerr << "Error opening file for reading: " << GetLastError() << std::endl;
                ReleaseMutex(WaitEvents[1]);
                continue;
            }

            outputFile.write(message.c_str(), MAX_LENGTH);
            outputFile.close();
            std::cout << "Message written to the file." << std::endl;
        }
        else
        {
            std::cout << "File is already containing " << max_record << " records." << std::endl;
        }
        ReleaseMutex(WaitEvents[1]);
    }

    CloseHandle(hEvent);
    CloseHandle(WaitEvents[0]);
    CloseHandle(WaitEvents[1]);
    return 0;
}