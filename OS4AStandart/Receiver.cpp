#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Receiver_func.h"

using namespace std;

const int MAX_LENGTH = 20;

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    int process_count;
    cout << "Enter count of Sender processes: ";
    cin >> process_count;

    int record_count;
    cout << "\nEnter max count of records in file: ";
    cin >> record_count;

    string filename;
    cout << "\nEnter binary file name: ";
    cin >> filename;
    filename += ".bin";
    CreateBinaryFile(filename);

    HANDLE hMutex = CreateMutex(NULL, FALSE, "Global\\MyGlobalMutex");
    if (hMutex == NULL)
    {
        std::cerr << "Error creating Mutex: " << GetLastError() << std::endl;
        return 1;
    }
    WaitForSingleObject(hMutex, INFINITE);
    for(int i = 0; i < process_count; i++)
    {
        CreateSenderProcess(filename, i, record_count);
    }
    HANDLE hControlEvent;
    vector<HANDLE> events;
    CreateEvents(hControlEvent, events, process_count);
    WaitForMultipleObjects(events.size(), events.data(), TRUE, INFINITE);
    ReleaseMutex(hMutex);
    while(true)
    {
        WaitForSingleObject(hMutex, INFINITE);
        cout << "Enter command ('Continue or exit'): ";
        string command;
        cin >> command;

        if (command == "exit")
        {
            SetEvent(hControlEvent);
            break;
        }
        else if (command == "Continue")
        {
            ReadBinaryFile(filename);
            ResetEvent(hControlEvent);
        }
        else
        {
            cout << "Unknown command, use Continue or exit.\n";
        }
        ReleaseMutex(hMutex);
    }

    // Cleanup
    for (auto& hEvent : events) {
        CloseHandle(hEvent);
    }
    CloseHandle(hControlEvent);
    CloseHandle(hMutex);

    cout << "The Work is done.\n";
    return 0;
}