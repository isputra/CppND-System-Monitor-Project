#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int idle=0, nonidle=0, i=0;
    now_cpus = LinuxParser::CpuUtilization();
    std::vector<std::string>::iterator iter_cpu; 
    for(iter_cpu = now_cpus.begin(); iter_cpu != now_cpus.end(); iter_cpu++, i++) {
        if(i == 3 || i == 4) {
            idle += std::stoi(*iter_cpu);
        } else {
            nonidle += std::stoi(*iter_cpu);
        }
    }
    return (float) nonidle / (nonidle + idle);
}