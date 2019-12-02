#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(Process const& a) const { 
    return stoi(this->ram_) > stoi(a.ram_); 
}

bool Process::SortByPID(Process const& a) const {
    return this->pid_ > a.pid_;
}

bool Process::SortByUser(Process const& a) const {
    return this->user_ > a.user_;
}

bool Process::SortByCpu(Process const& a) const {
    return this->cpu_ > a.cpu_;
}

bool Process::SortByUpTime(Process const& a) const {
    return this->upTime_ > a.upTime_;
}