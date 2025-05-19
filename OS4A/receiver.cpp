#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
namespace bip = boost::interprocess;
namespace bp = boost::process;

const int MAX_LENGTH = 20;

struct ControlData {
    bool terminate = false;
    bool allow_write = true;
};

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    
    // Clean up previous instances
    bip::shared_memory_object::remove("GlobalSM");
    bip::named_mutex::remove("GlobalMutex");
    bip::named_condition::remove("GlobalCond");

    // Create shared memory and sync objects
    bip::managed_shared_memory segment(bip::create_only, "GlobalSM", 65536);
    ControlData* ctrl = segment.construct<ControlData>("ControlData")();
    ctrl->terminate = false;
    ctrl->allow_write = true;

    bip::named_mutex mutex(bip::create_only, "GlobalMutex");
    bip::named_condition cond(bip::create_only, "GlobalCond");

    // Get parameters
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

    // Create empty file
    ofstream(filename, ios::binary | ios::trunc).close();

    // Start Sender processes
    vector<bp::child> senders;
    for(int i = 0; i < process_count; ++i) {
        senders.emplace_back(
            bp::search_path("Sender.exe"),
            filename,
            to_string(i),
            to_string(record_count)
        );
    }
    while(true)
    {
        {
            bip::scoped_lock<bip::named_mutex> lock(mutex);
            string cmd;
            cout << "Enter command ('Continue' or 'exit'): ";
            cin >> cmd;
            if(cmd == "exit")
            {
                ctrl->terminate = true;
                cond.notify_all();
                break;
            }
            else if(cmd == "Continue")
            {
                {
                    ifstream in(filename, ios::binary | ios::ate);
                    streamsize size = in.tellg();
                    in.seekg(0, ios::beg);
                    
                    if(size >= MAX_LENGTH) {
                        vector<char> buffer(MAX_LENGTH);
                        in.read(buffer.data(), MAX_LENGTH);
                        cout << "Message: " << string(buffer.begin(), buffer.end()) << endl;
                        vector<char> remaining(size - MAX_LENGTH);
                        in.read(remaining.data(), remaining.size());
                        in.close();
                        
                        ofstream outfile(filename, ios::binary | ios::trunc);
                        outfile.write(remaining.data(), remaining.size());
                        outfile.close();
                    }
                    else
                    {
                        cout << "No complete message available (need 20 bytes)\n";
                    }
                }
            }
            else
            {
                cout << "Unknown command\n";
            }
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    // Cleanup
    for(auto& s : senders)
    {
        if(s.running())
        {
            s.terminate();
        }
    }

    bip::shared_memory_object::remove("GlobalSM");
    bip::named_mutex::remove("GlobalMutex");
    bip::named_condition::remove("GlobalCond");

    cout << "Work completed\n";
    return 0;
}