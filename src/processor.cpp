#include <thread>
#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int idle=0, nonidle=0, i=0;
    int prev_idle=0, prev_nonidle=0;
    prev_cpus = CpuUtilization();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    now_cpus = CpuUtilization();
    std::vector<std::string>::iterator iter_cpu; 
    for(iter_cpu = now_cpus.begin(); iter_cpu != now_cpus.end(); iter_cpu++, i++) {
        if(i == CPUStates::kIdle_ || i == CPUStates::kIOwait_) {
            prev_idle += std::stoi(prev_cpus[i]);
            idle += std::stoi(*iter_cpu);
        } else {
            prev_nonidle += std::stoi(prev_cpus[i]);
            nonidle += std::stoi(*iter_cpu);
        }
    }
    idle = idle - prev_idle;
    nonidle = nonidle - prev_nonidle;
    return (float) nonidle / (nonidle + idle);
}