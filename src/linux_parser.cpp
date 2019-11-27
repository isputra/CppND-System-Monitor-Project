#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  std::unordered_map<string, long> umap_meminfo;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      umap_meminfo[key] = value;
    }
  }

  long totalMemoryUsed = umap_meminfo["MemTotal"] - umap_meminfo["MemFree"];
  // long buffers = umap_meminfo["Buffers"];
  // long cachedMemory = umap_meminfo["Cached"] + umap_meminfo["SReclaimable"] - umap_meminfo["Shmem"];
  // long nonCacheBufferMemory = totalMemoryUsed - (buffers + cachedMemory);
  // return (float)(nonCacheBufferMemory + buffers + cachedMemory)/(float)totalMemoryUsed;
  return (float) totalMemoryUsed / umap_meminfo["MemTotal"];
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime_system;
  long uptime_idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime_system >> uptime_idle;
  }
  return uptime_system;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return stol(CpuUtilization()[kUser_])
        + stol(CpuUtilization()[kNice_])
        + stol(CpuUtilization()[kSystem_])
        + stol(CpuUtilization()[kIRQ_])
        + stol(CpuUtilization()[kSoftIRQ_])
        + stol(CpuUtilization()[kSteal_])
        + stol(CpuUtilization()[kGuest_])
        + stol(CpuUtilization()[kGuestNice_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return stol(CpuUtilization()[kIdle_])
        + stol(CpuUtilization()[kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string value;
  string cpu;
  vector<string> v_cpu;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while(linestream >> value) {
      v_cpu.push_back(value);
    }
  }
  return v_cpu;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes")
        return value;
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running")
        return value;
    }
  }
  return 0;

}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmdline;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmdline);
  }
  return cmdline;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      stringstream >> key >> value;
      if (key == "VmSize:") {
        return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      stringstream >> key >> uid;
      if (key == "Uid:") {
        return uid;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string key;
  string user, x, suid;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);
      stringstream >> user >> x >> suid;
      if (suid.compare(uid) == 0) {
        return user;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  long starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      if (i == 22) {
        starttime = stol(value);
        break;
      }
      i++;
    }
  }
  long uptime = LinuxParser::UpTime();
  //std::cout << "Uptime: " << uptime << '\n';
  //std::cout << "starttime: " << starttime << '\n';
  //std::cout << "sysconf(_SC_CLK_TCK): " << sysconf(_SC_CLK_TCK) << '\n';
  //std::cout << "starttime / sysconf(_SC_CLK_TCK): " << (starttime / sysconf(_SC_CLK_TCK)) << '\n';
  //return (starttime / sysconf(_SC_CLK_TCK));
  return uptime - (starttime / sysconf(_SC_CLK_TCK));
}

long LinuxParser::Cpu(int pid) {
  string line, value;
  long total_time = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      if (i == 14 || i == 15 || i == 16 || i == 17) {
        total_time += stol(value);
      }
      if (i > 17) break;
      i++;
    }
  }
  long seconds = LinuxParser::UpTime(pid);
  return 100 * ( (total_time / sysconf(_SC_CLK_TCK)) / seconds );
}