#include <string>
#include "windows.h"
#include <vector>
#include <fstream>
void Input_Value(int& value, int lowerBound);

void Input_Value(std::string& value, int lowerBound);

void CreateEvents(HANDLE& hControl, std::vector<HANDLE>& events_ready, int process_count);

void CreateMutexes(HANDLE& hMutex);

void CreateSenderProcess(const std::string& filename, int index, int max_records);

void CreateBinaryFile(const std::string& filename);

void ReadBinaryFile(const std::string& filename);