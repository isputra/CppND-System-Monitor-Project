#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes(std::size_t sort_processes_by) {
    vector<int> pids(LinuxParser::Pids());
    vector<int>::iterator iter_pids;
    for (iter_pids = pids.begin(); iter_pids != pids.end(); iter_pids++) {
        Process p(*iter_pids);
        processes_.push_back(p);
    }
    switch (sort_processes_by)
    {
    case Process::ProcessColumn::cPID_:
        sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a.SortByPID(b);});
        break;
    case Process::ProcessColumn::cUser_:
        sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a.SortByUser(b);});
        break;
    case Process::ProcessColumn::cCPU_:
        sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a.SortByCpu(b);});
        break;
    case Process::ProcessColumn::cRAM_:
        sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a>b;});
        break;
    case Process::ProcessColumn::cUpTime_:
        sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a.SortByUpTime(b);});
        break;
    default:
        sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a>b;});
        break;
    }
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

vector<float> System::VectorMemoryUtilization() { return LinuxParser::VectorMemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }