#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h>
#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {
      int const freq = sysconf(_SC_CLK_TCK);

      user_ = LinuxParser::User(pid_);
      command_= LinuxParser::Command(pid_);

      ram_ = LinuxParser::Ram(pid_);
      upTime_ = LinuxParser::UpTime(pid_);
      cpu_ = 100 * ( (float)(LinuxParser::ActiveJiffies(pid_) / freq) / upTime_ );
  };
  std::string ram_;
 float cpu_;
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int pid_;
 std::string user_;
 std::string command_;
 long int upTime_;

};

#endif